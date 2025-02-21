#include "Link.h"

Link::Link(Node* from, Node* to, long long capacity)
:from(from), to(to), capacity(capacity), used_capacity(0){}

const Node *Link::getFrom()const{
    return from;
}
const Node *Link::getTo()const{
    return to;
}

string Link::getName() const {
    return "Link[ " + from->getName() + ", " + to->getName() + " ]";

}
long long Link::getRemainCapacity() const {
    return capacity - used_capacity;
}
bool Link::isAble() const {
    return getRemainCapacity() > 0;
}
void Link::assign(Data* data){
    DEBUG("Link::assign : " << data->getName() << " with " << this->getName() <<endl);
    assert(getRemainCapacity() > 0);
    used_capacity++;
    to->dataIn(data);
}
void Link::transmit(Data* data){
    cout<<"Link::transmit : (" << data->getGrid() << "," << data->getDataCnt() << ") from " << from->getId() << " to " << to->getId() <<endl;
    // from->dataOut(data); // because it is a TEG
    to->dataIn(data);
    used_capacity--;
}
