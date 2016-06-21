import std.stream;
import std.stdio;
import std.string;
import std.conv;

class Node{
  int Nodenumber;
  int[] PairingNode;
  int[] RoadCost;
  int Edgenumber;
public:
  this(int x){
    Nodenumber = x;
    Edgenumber = 0;
    PairingNode.length = 0;
    RoadCost.length = 0;
  }
  this(){
    Edgenumber = 0;
    PairingNode.length = 0;
    RoadCost.length = 0;
  };
  //~this(){delete PairingNode; };
  void Nnumber(int x){Nodenumber=x;};
  int Nnumber() const{return Nodenumber;};
  void set_PNode(int newNode, int c){
    PairingNode.length += 1;
    PairingNode[Edgenumber] = newNode;
    RoadCost.length += 1;
    RoadCost[Edgenumber++] = c;
  };
  int get_PNode(int x) { return PairingNode[x];};
  int get_RCost(int x) { return RoadCost[x];};
  int Enumber(){return Edgenumber;}
};

void main(string[] args){
  int start = to!(int)(args[1]);
  int end = to!(int)(args[2]);
  end = (end==99) ? 51 : end;
  Node[52] N;
  for(uint i = 1; i<=51; i++){
    N[i] = new Node;
  }
  for(uint i = 1; i<=50; i++){
    N[i].Nnumber = i;
  }
  N[51].Nnumber = 99;
  //writeln("oshiri!");

  int[231] PN1;
  int[231] PN2;
  int[231] cost;
  int j=0;
  while( !stdin.eof() ){
    readf("%d,%d,%d\n", &PN1[j], &PN2[j], &cost[j]);
    //writefln("%d, %d,%d,%d", j, PN1[j], PN2[j], cost[j]);
    if(PN1[j]==99){
      PN1[j]=51;
    }else if(PN2[j]==99){
      PN2[j]=51;
    }
    j++;
  }
  j -= 1;
  //writeln(j);
  for(uint i = 0; i < j; i++){
    int a = PN1[i];
    int b = PN2[i];
    int c = cost[i];
    N[a].set_PNode(b,c);
    N[b].set_PNode(a,c);
  }
  bool[52][200] flag_count = false;
  bool[52] flag_confirmMin = false;
  int[52] minDistance = 0;
  int distance_max = 0;
  int dis = 0;
  int n;
  flag_count[0][start] = true;
  flag_confirmMin[start] = true;
  minDistance[start] = dis;
  for (uint i = 0; i < N[start].Enumber; i++) {
    flag_count[ dis+N[start].get_RCost(i) ][ N[start].get_PNode(i) ] = true;
  }

  while(1){
    int candidate;
    int candidate_distance = 200;
    for(n = 1; n <= 51; n++){
      if(flag_confirmMin[n]) continue;
      //writeln(n);
      for (dis = 1 ; dis < 200; dis++){
        //writeln(dis);
        if(flag_count[dis][n]){
          if(candidate_distance > dis){
            candidate = n;
            candidate_distance = dis;
          }
          break;
        }
      }
    }
    //writeln(n, dis);
    flag_confirmMin[candidate] = true;
    minDistance[candidate] = candidate_distance;
    uint k = 0;
    while(k<=51){
      if(!flag_confirmMin[k]) break;
      k++;
    }
    if(k == 52) break;
    for (uint i = 0; i < N[candidate].Enumber; i++) {
      flag_count[candidate_distance+N[candidate].get_RCost(i)][N[candidate].get_PNode(i)] = true;
    }
  }
  //writeln(minDistance[end]);
  int[] root;
  root.length = 0;
  int act = end;
  while(1){
    int next;
    //writeln(act);
    for(uint i = 0; i < N[act].Enumber; i++){
      if(N[act].get_RCost(i) == minDistance[act] - minDistance[N[act].get_PNode(i)]){
        next = N[act].get_PNode(i);
      }
    }
    root.length++;
    root[root.length-1] = (act == 51) ? 99 : act;
    act = next;
    if(next == start) break;
  }
  writef("%d->",start);
  for(int i = cast(int)(root.length-1) ; i > 0; i--){
    writef("%d->", root[i]);
  }
  writef("%d, Cost: %d\n", root[0], minDistance[end]);
}
