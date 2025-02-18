#include "Link.h"

Link::Link(Node* from, Node* to, long long capacity)
:from(from), to(to), capacity(capacity), used_capacity(0){}

const Node *Link::getFrom()const{
    return from;
}
const Node *Link::getTo()const{
    return to;
}
    
long long Link::getRemainCapacity() const {
    return capacity - used_capacity;
}
void Link::assign(Data* data){
    used_capacity++;
    data->pathAdd({to->getId(), from->getId()});
}
void Link::transmit(Data* data){
    cout<<"Transmit Data(" << data->getGrid() << "," << data->getDataCnt() << ") from " << from->getId() << " to " << to->getId() <<endl;
    // from->dataOut(data); // because it is a TEG
    to->dataIn(data);
    used_capacity--;
}
