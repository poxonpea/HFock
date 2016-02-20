#include "State.hh"
State::State(){
  state_index = 0;
  n = 1;
  l = 0;
  j2 = 1;
  mj2 = 1;
  tz2 = 1;
}

State::~State(){
}

State::State(int state_index, int n, int l, int j2, int mj2, int tz2){
  this->state_index = state_index; 
  this->n =  n;
  this->l = l;
  this->j2 = j2;
  this->mj2 = mj2;
  this->tz2 = tz2;
}
double State::GetEnergy(){
  const double HBARW = 10;//MeV
  return (2*n+l+(3./2.))*HBARW;//MeV
}
