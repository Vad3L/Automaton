#include "Automaton.h"
#include <cstdio>
#include <algorithm>
#include <cctype>
#include <ostream>
#include <iostream>


namespace fa {

  Automaton::Automaton() {}

  bool Automaton::isValid() const {
    return (countSymbols() && countStates()) ? true : false;
  }

  bool Automaton::addSymbol(char symbol){
    if(!std::isgraph(symbol)){return false;}
    return alphabet.insert(symbol).second;
  }

  bool Automaton::removeSymbol(char symbol){
    if(alphabet.erase(symbol) != 1){
      return false;
    }
    for (auto it = transitions.begin(); it != transitions.end();){
      if (it->first.second == symbol)
        it = transitions.erase(it);
      else
        ++it;
    }
    
    return true;
  }

  bool Automaton::hasSymbol(char symbol) const{
    return (alphabet.find(symbol) != alphabet.end());
  }

  std::size_t Automaton::countSymbols() const{
    return alphabet.size();
  }

  bool Automaton::addState(int state){
    if(state<0){return false;}
    return states.insert({state,std::make_pair(false,false)}).second;
  }

  bool Automaton::removeState(int state){

    if(states.erase(state) != 1){
      return false;
    }
    for (auto it = transitions.begin(); it != transitions.end();){
      if (it->first.first == state){
        it = transitions.erase(it);
      }else{
        removeTransition(it->first.first,it->first.second,state);
        it++;
      }
      
      
    }

    return true;
  } 

  
  bool Automaton::hasState(int state) const{
    return states.find(state) != states.end();
  }

  std::size_t Automaton::countStates() const{
    return states.size(); 
  }

  void Automaton::setStateInitial(int state){
    if(hasState(state)){states.at(state).first=true;}
  }


  bool Automaton::isStateInitial(int state) const{
    return (hasState(state) ? states.at(state).first : false);
  }

  void Automaton::setStateFinal(int state){
    if(hasState(state)){states.at(state).second=true;}
  }

  bool Automaton::isStateFinal(int state) const{
    return (hasState(state) ? states.at(state).second : false);
  }



  bool Automaton::addTransition(int from, char alpha, int to){
    if(!hasState(from) || !hasState(to)){
      //std::printf("1\n");
      return false;
    }

    if(hasTransition(from,alpha,to)){
      //std::printf("2\n");
      return false;
    }


    if(!hasSymbol(alpha) && fa::Epsilon != alpha){
      //std::printf("3 - %d\n",hasSymbol(alpha));
      return false;
    }

    auto result = transitions.find({from,alpha});

    if(result == transitions.end()){
      transitions.insert({std::make_pair(from,alpha), {to}});
    }else{
      result->second.push_back(to);
    }

    return true;
  }

  bool Automaton::removeTransition(int from, char alpha, int to){
    if(!hasTransition(from,alpha,to)){
      return false;
    }

    auto result = transitions.find({from,alpha});
    auto iterator = std::find(result->second.begin(),result->second.end(),to);

    return result->second.erase(iterator) != result->second.end();
  }


  bool Automaton::hasTransition(int from, char alpha, int to) const{
    auto result = transitions.find({from,alpha});

    
    if(result == transitions.end()){return false;}
    auto iterator = std::find(result->second.begin(),result->second.end(),to);
    

    return (iterator != result->second.end());
  }

  std::size_t Automaton::countTransitions() const{
    size_t nbTransition=0;
    
    for(auto i = transitions.begin() ; i != transitions.end(); ++i){
      nbTransition+= i->second.size();
    }

    return nbTransition;
  }

  void Automaton::prettyPrint(std::ostream& os) const{
    std::vector<int> initState;
    std::vector<int> finalState;
    std::vector<std::vector<int>> tabState;


    for (std::pair<int, std::pair<bool,bool>> i : states) {
      if (isStateInitial(i.first)){
        initState.push_back(i.first);
      }
      if (isStateFinal(i.first)){
        finalState.push_back(i.first);
      }

    }

    os << "Initial states:\n\t";
    for(int i : initState){
      os << i <<' ';
    } 
    os << std::endl;

    os << "Final states:\n\t"; 
    for(int i : finalState){
      os << i <<' ';
    } 
    os << std::endl;
   
    os << "Transitions"<< std::endl;
    int previous = -1 ;
    for (auto i = transitions.begin(); i != transitions.end(); i++) {
      if(i->first.first != previous){
        os << "\tFor state " << i->first.first << ":\n";
      }
      
      
      os << "\t\tFor letter " <<(i->first.second == fa::Epsilon ? '~': i->first.second) << ": ";
      for (size_t j = 0; j < i->second.size(); j++) {
      
        os << i->second[j] << " ";
        
      }
      os << std::endl;
      previous = i->first.first;
    }
  }

  void Automaton::dotPrint(std::ostream& os) const {
    os << "digraph Automaton {" << std::endl;
    os << "\t rankdir=LR" << std::endl;
    for (auto i : states) {
        if (isStateInitial(i.first) && isStateFinal(i.first)) {
            os << "\tn" << i.first << "[shape=none label=\"\" height=0 width=0 ]"<<std::endl;
            os << "\tn" << i.first << "->" << i.first << std::endl;
            os << "\t" << i.first << "[shape=doublecircle]" << std::endl;
        } else if (isStateInitial(i.first)) {
            os << "\tn" << i.first << "[shape=none label=\"\" height=0 width=0 ]"<<std::endl;
            os << "\tn" << i.first << "->" << i.first << std::endl;
            os << "\t" << i.first << "[shape=circle]" << std::endl;
        } else if (isStateFinal(i.first)) {
            os << "\t" << i.first;
            os << "[shape=doublecircle]" << std::endl;
        }
    }
    os << "\t" << "node [shape = circle]"<<std::endl;
    for (auto i : states){
      os << "\t" << i.first << std::endl ;
    }
    for (auto it = transitions.begin(); it != transitions.end(); it++) {
      for (size_t j = 0; j < it->second.size(); j++) {
        os << "\t" << it->first.first << " -> " << it->second[j] << " [label=\"" << (it->first.second == fa::Epsilon ? '~': it->first.second) << "\"]" << std::endl;
      }
      
    }
    
    os << "}" << std::endl;
  }

  bool Automaton::hasEpsilonTransition () const {
    for (auto it = transitions.begin(); it != transitions.end(); it++){
      if (it->first.second == fa::Epsilon){
        return true;
      }
    }
    return false;
  }

  bool Automaton::isDeterministic() const{
    int nbEtatInit = 0 ;
    for (auto i : states) {
      if(i.second.first){
        nbEtatInit++;
      }
    }
    
    if(nbEtatInit != 1){return false;}

    for (auto it = transitions.begin(); it != transitions.end();++it){
      if (it->second.size() > 1){
        return false;
      }
    }

    return true;
  }

  bool Automaton::isComplete() const{

    for (auto i : states){
      for (auto itt : alphabet){
        if (transitions.find({i.first,itt}) == transitions.end()){
          return false;
        }
      }      
    }

    return true;
  }

	void Automaton::removeNonAccessibleStates(){

	}

	void Automaton::removeNonCoAccessibleStates(){
	
	}
	

	bool Automaton::DepthFirstSearch_empty(std::set<int> v,int s ){
		v.insert(s);
		for( auto i : transitions){
			if(i.first.first == s){
				for(int x : i.second){
					if(isStateFinal(x)){
						return false;
					}
					if(v.find(x) == v.end()){
						return DepthFirstSearch_empty(v,x);
					}
				}
			}
		}
		return true;
	}

	bool Automaton::isLanguageEmpty() const{
		bool init = false;
		bool final = false ;												
		std::vector<int> tabStateInit;	
		std::vector<int> tabStateFinal;					
		for(auto i : states){
			if(i.second.first){
				tabStateInit.push_back(i.first);
				init = true;
			}
			if(i.second.second){
				tabStateFinal.push_back(i.first);
				final = true;
			}
		}
		if(!init || !final){
			std::cout << "true" << std::endl;
			return true;
		}
		
		std::set<int> v;
		for(auto x : tabStateInit){
			if(!DepthFirstSearch_empty(v,x)){
				return false;
			}	
		}

		return true;
	}

  bool Automaton::hasEmptyIntersectionWith(const Automaton& other) const{
    return false;
  }

  std::set<int> Automaton::readString(const std::string& word) const{
    std::set<int> sI;
    return sI;
  }

  bool Automaton::match(const std::string& word) const{
    return false;
  }

  bool Automaton::isIncludedIn(const Automaton& other) const{
    return false;
  }

  Automaton Automaton::createMirror(const Automaton& automaton){
    fa::Automaton bigBrother;
    bigBrother.states=automaton.states;
    bigBrother.alphabet=automaton.alphabet;

    for( auto it : automaton.transitions){
      for(int i = 0 ; i < it.second.size(); ++i){
        bigBrother.addTransition(it.second[i],it.first.second,it.first.first);
      }
    }

    return bigBrother;
  }

  Automaton Automaton::createComplete(const Automaton& automaton){
    if (automaton.isComplete()){
      return automaton;
    }

    fa::Automaton create = automaton;

    int binState = 0;
    while(!create.addState(binState)){
      binState++;
    }

    for (auto itt : create.alphabet){
      create.addTransition(binState,itt,binState);
    }

    for (auto i : automaton.states){
      for (auto itt : automaton.alphabet){
        if (automaton.transitions.find({i.first,itt}) == automaton.transitions.end()){
          create.addTransition(i.first,itt,binState);
        }
      } 
    }

    return create;
  }


  Automaton Automaton::createComplement(const Automaton& automaton){
    fa::Automaton  glados = automaton;
    //if(!glados.isDeterministic()){glados = glados.deterministic();}
    if(!glados.isComplete()){glados = glados.createComplete(glados);}

    for(int i = 0 ; i < glados.states.size(); ++i){
      (glados.states[i].second ? glados.states[i].second = false: glados.states[i].second = true );
    }
      
    return glados;
  }


  Automaton Automaton::createProduct(const Automaton& lhs, const Automaton& rhs){
    return rhs;
  }


  Automaton Automaton::createDeterministic(const Automaton& other){
    return other;
  }


  Automaton Automaton::createMinimalMoore(const Automaton& other){
    return other;
  }


  Automaton Automaton::createMinimalBrzozowski(const Automaton& other){
    return other;
  }



}
