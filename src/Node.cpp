#include "Node.h"

Node::Node(int id, int timeslot, int type, long long capacity)
    :id(id), timeslot(timeslot), type(type), capacity(capacity), used_capacity(0){
}

int Node::getId() const {
    return id;
}
int Node::getType() const {
    return type;
}
long long Node::getCapacity()const{
    return capacity;
}
long long Node::getUsedCapacity()const{
    return used_capacity;
}


void Node::dataIn(Data* data){
    int grid_id = data->getGrid();
    if(data->getType() == Data::TREE) {
        trees[grid_id] = data;
    } else {
        raw_datas[grid_id].push(data);
    }

    used_capacity += data->getSize();
}

void Node::dataOut(Data* data){
    int grid_id = data->getGrid();
    if(data->getType() == Data::TREE) {
        trees.erase(grid_id);
    } else {
        raw_datas[grid_id].push(data);
    }
    
    used_capacity += data->getSize();
}



User::User(int id, int grid, int timeslot)
    : Node(id, timeslot, Node::USER), grid(grid){}

int User::getGrid() const {
    return grid;
}

Satellite::Satellite(int id, int timeslot, long long storage)
    :Node(id, timeslot, Node::SATELLITE, storage){}