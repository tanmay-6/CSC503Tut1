#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

enum class OpKind { Send, Recv, Print };

struct Operation {
    OpKind kind{};
    string peer;
    string payload;
};

struct Process {
    string id;
    vector<Operation> program;
    int ip = 0;
    long long time = 0;
};

static bool looksLikeProc(const string& s) {
    if (s.size() < 2 || s[0] != 'p') return false;
    if (!all_of(s.begin() + 1, s.end(), [](unsigned char c){ return std::isdigit(c); })) return false;
    int k = stoi(s.substr(1));
    return (1 <= k && k <= 9);
}

static bool isAlphaNumToken(const string& s) {
    if (s.empty()) return false;
    for (unsigned char c : s) {
        if (!std::isalnum(c)) return false;
    }
    return true;
}

static void trimInPlace(string& s) {
    size_t L = 0, R = s.size();
    while (L < R && std::isspace(static_cast<unsigned char>(s[L]))) ++L;
    while (R > L && std::isspace(static_cast<unsigned char>(s[R - 1]))) --R;
    if (L == 0 && R == s.size()) return;
    s = s.substr(L, R - L);
}

struct MsgKey {
    string from;
    string to;
    string data;
    bool operator==(const MsgKey& o) const {
        return from == o.from && to == o.to && data == o.data;
    }
};

struct MsgKeyHash {
    size_t operator()(const MsgKey& k) const noexcept {
        size_t h1 = std::hash<string>{}(k.from);
        size_t h2 = std::hash<string>{}(k.to);
        size_t h3 = std::hash<string>{}(k.data);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Slurp all input lines
    vector<string> lines;
    for (string line; getline(cin, line); ) {
        lines.push_back(line);
    } // [web:8]

    // Parse into processes
    unordered_map<string, Process> procMap;
    vector<string> orderIds;
    bool inside = false;
    string currentId;

    auto fail = [&](const string& msg) -> int {
        cerr << "error: " << msg << "\n";
        return 0;
    }; // [web:8]

    int lineNo = 0;
    for (const string& raw : lines) {
        ++lineNo;
        string line = raw;
        trimInPlace(line);
        if (line.empty()) continue;

        // tokenization
        vector<string> tok;
        {
            istringstream iss(line);
            string t;
            while (iss >> t) tok.push_back(t);
        }

        if (tok.empty()) continue;

        if (!inside) {
            // expect: begin process pK
            if (tok.size() >= 3 && tok[0] == "begin" && tok[1] == "process") {
                const string nm = tok[2];
                if (!looksLikeProc(nm)) {
                    cerr << "error: invalid process name '" << nm << "'\n";
                    return 0;
                }
                if (procMap.count(nm)) {
                    cerr << "error: duplicate process '" << nm << "'\n";
                    return 0;
                }
                inside = true;
                currentId = nm;
                Process P;
                P.id = nm;
                procMap.emplace(nm, std::move(P));
                orderIds.push_back(nm);
            } else {
                cerr << "error: expected 'begin process pK'\n";
                return 0;
            }
        } else {
            // inside a process
            if (tok.size() >= 2 && tok[0] == "end" && tok[1] == "process") {
                inside = false;
                currentId.clear();
                continue;
            }

            // operations: print X | send pN X | recv pN X
            if (tok[0] == "print") {
                if (tok.size() != 2) {
                    cerr << "error: syntax error: expected 'print msg'\n";
                    return 0;
                }
                const string msg = tok[1];
                if (!isAlphaNumToken(msg)) {
                    cerr << "error: invalid message token '" << msg << "'\n";
                    return 0;
                }
                Operation op;
                op.kind = OpKind::Print;
                op.payload = msg;
                procMap[currentId].program.push_back(op);
            } else if (tok[0] == "send") {
                if (tok.size() != 3) {
                    cerr << "error: syntax error: expected 'send pN msg'\n";
                    return 0;
                }
                const string peer = tok[1];
                const string msg = tok[2];
                if (!looksLikeProc(peer)) {
                    cerr << "error: invalid process name '" << peer << "'\n";
                    return 0;
                }
                if (!isAlphaNumToken(msg)) {
                    cerr << "error: invalid message token '" << msg << "'\n";
                    return 0;
                }
                Operation op;
                op.kind = OpKind::Send;
                op.peer = peer;
                op.payload = msg;
                procMap[currentId].program.push_back(op);
            } else if (tok[0] == "recv") {
                if (tok.size() != 3) {
                    cerr << "error: syntax error: expected 'recv pN msg'\n";
                    return 0;
                }
                const string peer = tok[1];
                const string msg = tok[2];
                if (!looksLikeProc(peer)) {
                    cerr << "error: invalid process name '" << peer << "'\n";
                    return 0;
                }
                if (!isAlphaNumToken(msg)) {
                    cerr << "error: invalid message token '" << msg << "'\n";
                    return 0;
                }
                Operation op;
                op.kind = OpKind::Recv;
                op.peer = peer;
                op.payload = msg;
                procMap[currentId].program.push_back(op);
            } else {
                cerr << "error: syntax error: unknown operation '" << tok[0] << "'\n";
                return 0;
            }
        }
    }

    if (inside) {
        cerr << "error: missing 'end process'\n";
        return 0;
    }
    if (procMap.empty()) {
        cerr << "error: no processes defined\n";
        return 0;
    }

    // Round-robin schedule
    vector<Process*> schedule;
    schedule.reserve(orderIds.size());
    for (const auto& nm : orderIds) {
        schedule.push_back(&procMap[nm]);
    }

    // Message multimap: (from,to,msg) -> queue of sender timestamps
    unordered_map<MsgKey, deque<long long>, MsgKeyHash> inbox;

    vector<string> log;

    auto everyoneDone = [&]() {
        for (auto* p : schedule) {
            if (p->ip < static_cast<int>(p->program.size())) return false;
        }
        return true;
    }; // [web:8]

    // Fair simulation loop
    bool isDeadlocked = false;
    while (true) {
        if (everyoneDone()) break;
        bool step = false;

        for (auto* p : schedule) {
            if (p->ip >= static_cast<int>(p->program.size())) continue;
            Operation& op = p->program[p->ip];

            if (op.kind == OpKind::Print) {
                p->time += 1;
                {
                    ostringstream oss;
                    oss << "printed " << p->id << " " << op.payload << " " << p->time;
                    log.push_back(oss.str());
                }
                p->ip += 1;
                step = true;
                break;
            } else if (op.kind == OpKind::Send) {
                p->time += 1;
                MsgKey key{p->id, op.peer, op.payload};
                inbox[key].push_back(p->time);
                {
                    ostringstream oss;
                    oss << "sent " << p->id << " " << op.payload << " " << op.peer << " " << p->time;
                    log.push_back(oss.str());
                }
                p->ip += 1;
                step = true;
                break;
            } else { // Recv
                MsgKey key{op.peer, p->id, op.payload};
                auto it = inbox.find(key);
                if (it != inbox.end() && !it->second.empty()) {
                    long long ts = it->second.front();
                    it->second.pop_front();
                    if (it->second.empty()) inbox.erase(it);
                    p->time = std::max(p->time, ts) + 1;
                    {
                        ostringstream oss;
                        oss << "received " << p->id << " " << op.payload << " " << op.peer << " " << p->time;
                        log.push_back(oss.str());
                    }
                    p->ip += 1;
                    step = true;
                    break;
                } else {
                    continue;
                }
            }
        }

        if (!step) {
            isDeadlocked = true;
            break;
        }
    }

    for (const auto& s : log) {
        cout << s << "\n";
    }

    if (isDeadlocked) {
        cout << "system deadlocked\n";
        return 0;
    }

    for (auto& kv : inbox) {
        const MsgKey& k = kv.first;
        auto pending = kv.second.size();
        for (size_t i = 0; i < pending; ++i) {
            cout << "message " << k.data << " " << k.from << " " << k.to << " not delivered\n";
        }
    }

    return 0;
}