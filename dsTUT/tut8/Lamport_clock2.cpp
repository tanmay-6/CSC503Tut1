#include <bits/stdc++.h>
using namespace std;

struct Operation {
    enum Type { SEND, RECV, PRINT } type;
    string peer;
    string msg;
};

struct Process {
    string name;
    vector<Operation> ops;
    int pc = 0;
    long long clock = 0;
};

struct Msg {
    string from, to, payload;
    long long ts;
};

static bool isProcName(const string& s) {
    if (s.size() < 2 || s[0] != 'p') return false;
    if (!all_of(s.begin()+1, s.end(), ::isdigit)) return false;
    int k = stoi(s.substr(1));
    return (1 <= k && k <= 9);
}

static bool isAlphaNum(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isalnum(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Reading all lines
    vector<string> lines;
    for (string line; getline(cin, line); ) {
        lines.push_back(line);
    }

    // Parsing
    unordered_map<string, Process> procs;
    vector<string> procOrder;
    bool inProc = false;
    string curName;
    int lineNo = 0;

    auto error = [&](const string& msg){
        cerr << "error: " << msg << "\n";
        return 0;
    };

    for (const string& raw : lines) {
        lineNo++;
        string line = raw;
        auto ltrim = [](string& s){ // trim leading/trailing spaces
            size_t i = 0;
            while (i < s.size() && isspace(static_cast<unsigned char>(s[i]))) i++;
            s.erase(0, i);
        };
        auto rtrim = [](string& s){
            size_t i = s.size();
            while (i > 0 && isspace(static_cast<unsigned char>(s[i-1]))) i--;
            s.erase(i);
        };
        ltrim(line); rtrim(line);
        if (line.empty()) continue;

        // Tokenize
        vector<string> tok;
        {
            istringstream iss(line);
            string t;
            while (iss >> t) tok.push_back(t);
        }
        if (tok.empty()) continue;

        if (!inProc) {
            // Expect either begin process pK or ignore blank
            if (tok.size() >= 3 && tok[0] == "begin" && tok[1] == "process") {
                string nm = tok[2];
                if (!isProcName(nm)) {
                    cerr << "error: invalid process name '" << nm << "'\n";
                    return 0;
                }
                if (procs.count(nm)) {
                    cerr << "error: duplicate process '" << nm << "'\n";
                    return 0;
                }
                inProc = true;
                curName = nm;
                Process P;
                P.name = nm;
                procs[nm] = P;
                procOrder.push_back(nm);
            } else {
                cerr << "error: expected 'begin process pK'\n";
                return 0;
            }
        } else {
            // Inside process: expect ops or end process
            if (tok.size() >= 2 && tok[0] == "end" && tok[1] == "process") {
                inProc = false;
                curName.clear();
                continue;
            }
            // parse operation
            if (tok[0] == "print") {
                if (tok.size() != 2) {
                    cerr << "error: syntax error: expected 'print msg'\n";
                    return 0;
                }
                string msg = tok[1];
                if (!isAlphaNum(msg)) {
                    cerr << "error: invalid message token '" << msg << "'\n";
                    return 0;
                }
                Operation op;
                op.type = Operation::PRINT;
                op.msg = msg;
                procs[curName].ops.push_back(op);
            } else if (tok[0] == "send") {
                if (tok.size() != 3) {
                    cerr << "error: syntax error: expected 'send pN msg'\n";
                    return 0;
                }
                string peer = tok[1], msg = tok[2];
                if (!isProcName(peer)) {
                    cerr << "error: invalid process name '" << peer << "'\n";
                    return 0;
                }
                if (!isAlphaNum(msg)) {
                    cerr << "error: invalid message token '" << msg << "'\n";
                    return 0;
                }
                Operation op;
                op.type = Operation::SEND;
                op.peer = peer;
                op.msg = msg;
                procs[curName].ops.push_back(op);
            } else if (tok[0] == "recv") {
                if (tok.size() != 3) {
                    cerr << "error: syntax error: expected 'recv pN msg'\n";
                    return 0;
                }
                string peer = tok[1], msg = tok[2];
                if (!isProcName(peer)) {
                    cerr << "error: invalid process name '" << peer << "'\n";
                    return 0;
                }
                if (!isAlphaNum(msg)) {
                    cerr << "error: invalid message token '" << msg << "'\n";
                    return 0;
                }
                Operation op;
                op.type = Operation::RECV;
                op.peer = peer;
                op.msg = msg;
                procs[curName].ops.push_back(op);
            } else {
                cerr << "error: syntax error: unknown operation '" << tok[0] << "'\n";
                return 0;
            }
        }
    }
    if (inProc) {
        cerr << "error: missing 'end process'\n";
        return 0;
    }
    if (procs.empty()) {
        cerr << "error: no processes defined\n";
        return 0;
    }

    // Build index order for round-robin
    vector<Process*> order;
    order.reserve(procOrder.size());
    for (auto& nm : procOrder) {
        order.push_back(&procs[nm]);
    }

    // Message pool: multimap keyed by (from,to,msg) with queue of timestamps
    struct Key {
        string from, to, msg;
        bool operator==(const Key& o) const {
            return from==o.from && to==o.to && msg==o.msg;
        }
    };
    struct KeyHash {
        size_t operator()(Key const& k) const noexcept {
            size_t h1 = std::hash<string>{}(k.from);
            size_t h2 = std::hash<string>{}(k.to);
            size_t h3 = std::hash<string>{}(k.msg);
            return h1 ^ (h2<<1) ^ (h3<<2);
        }
    };
    unordered_map<Key, deque<long long>, KeyHash> pool;

    vector<string> logLines;

    auto allFinished = [&](){
        for (auto p : order) if (p->pc < (int)p->ops.size()) return false;
        return true;
    };

    // Simulation loop: fair round-robin, execute 1 enabled op per cycle
    bool deadlocked = false;
    while (true) {
        if (allFinished()) break;
        bool progressed = false;

        for (auto p : order) {
            if (p->pc >= (int)p->ops.size()) continue;
            Operation& op = p->ops[p->pc];

            if (op.type == Operation::PRINT) {
                p->clock += 1;
                // printed pK msg T
                {
                    ostringstream oss;
                    oss << "printed " << p->name << " " << op.msg << " " << p->clock;
                    logLines.push_back(oss.str());
                }
                p->pc += 1;
                progressed = true;
                break; // execute exactly one event per scheduling cycle
            } else if (op.type == Operation::SEND) {
                p->clock += 1;
                // enqueue message with sender timestamp
                Key k{p->name, op.peer, op.msg};
                pool[k].push_back(p->clock);
                // sent pN msg pM T
                {
                    ostringstream oss;
                    oss << "sent " << p->name << " " << op.msg << " " << op.peer << " " << p->clock;
                    logLines.push_back(oss.str());
                }
                p->pc += 1;
                progressed = true;
                break;
            } else if (op.type == Operation::RECV) {
                Key k{op.peer, p->name, op.msg};
                auto it = pool.find(k);
                if (it != pool.end() && !it->second.empty()) {
                    long long ts_send = it->second.front();
                    it->second.pop_front();
                    if (it->second.empty()) pool.erase(it);
                    p->clock = max(p->clock, ts_send) + 1;
                    // received pN msg pM T
                    {
                        ostringstream oss;
                        oss << "received " << p->name << " " << op.msg << " " << op.peer << " " << p->clock;
                        logLines.push_back(oss.str());
                    }
                    p->pc += 1;
                    progressed = true;
                    break;
                } else {
                    // not enabled; continue to next process
                    continue;
                }
            }
        }

        if (!progressed) {
            // No enabled op in a full round, not all finished => deadlock
            deadlocked = true;
            break;
        }
    }

    // Print log
    for (auto& s : logLines) {
        cout << s << "\n";
    }

    if (deadlocked) {
        cout << "system deadlocked\n";
        return 0;
    }

    // After normal finish, list undelivered messages
    for (auto& kv : pool) {
        const Key& k = kv.first;
        auto cnt = kv.second.size();
        for (size_t i = 0; i < cnt; ++i) {
            // message m2 p2 p1 not delivered
            cout << "message " << k.msg << " " << k.from << " " << k.to << " not delivered\n";
        }
    }

    return 0;
}