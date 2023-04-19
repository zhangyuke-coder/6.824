 #include "../Util.h"
 #include "../Raft.h"
 #include <vector>
 #define COMMOM_PORT 1234
 int main(int argc, char* argv[]) {
    if(argc < 2){
        printf("loss parameter of peersNum\n");
        exit(-1);
    }
    int peersNum = atoi(argv[1]);
    if(peersNum % 2 == 0){
        printf("the peersNum should be odd\n");  //必须传入奇数，这是raft集群的要求
        exit(-1);
    }
    srand((unsigned)time(NULL));
    std::vector<PeersInfo> peers(peersNum);
    for(int i = 0; i < peersNum; i++){
        peers[i].m_peerId = i;
        peers[i].electionPort = COMMOM_PORT + i;                    //vote的RPC端口
        peers[i].logPort = COMMOM_PORT + i + peers.size();    //append的RPC端口
        // printf(" id : %d port1 : %d, port2 : %d\n", peers[i].m_peerId, peers[i].m_port.first, peers[i].m_port.second);
    }

    Raft* raft = new Raft[peers.size()];
    for(int i = 0; i < peers.size(); i++){
        raft[i].Make(peers, i);
    }
    
 }
 
