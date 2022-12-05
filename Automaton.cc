#include "Automaton.h"
#include <cstdio>
#include <algorithm>
#include <cctype>
#include <ostream>
#include <iostream>
#include <fstream>

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
      if (it->first.second == symbol){
        it = transitions.erase(it);
	  }else{
        ++it;
      }
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
	
	for(auto a : alphabet){
		transitions.erase({state,a});
	}
    
	std::set<std::pair<int,char>> rem ;
	for(auto f : transitions){
		for(auto a : f.second){
			if(a == state){
				rem.insert(f.first);
			}
		}
	}

	for(auto e : rem ){
		removeTransition(e.first,e.second,state);
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
      return false;
    }

    if(hasTransition(from,alpha,to)){
      return false;
    }


    if(!hasSymbol(alpha) && fa::Epsilon != alpha){
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

	if(result->second.size() == 1){
		transitions.erase({from,alpha});
		return true;
	}
	
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
		std::set<int>  r;
		for(auto f : states){
			r.insert(f.first);
		}
		std::set<int> si = getInitialState();
		if(si.size() == 0){
			for(auto a : r ){
				removeState(a);
			}
			addState(0);
			setStateInitial(0);
			return;
		}
		
		std::set<int> visited;
		for(int lo : si){
			DepthFirstSearch(visited,lo);
		}
		std::set<int> notVisited;
		std::set_difference(begin(r),end(r),begin(visited),end(visited),inserter(notVisited,end(notVisited)));
		
		for(auto fe:notVisited){
			removeState(fe);
		}

	}

	void Automaton::removeNonCoAccessibleStates(){
		*this=createMirror(*this);
		removeNonAccessibleStates();
		*this=createMirror(*this);
	}
	
	void Automaton::DepthFirstSearch(std::set<int>& v,int s )const{
		v.insert(s);
		for( auto i : transitions){
			if(i.first.first == s){
				for(int x : i.second){
					if(v.find(x) == v.end()){
						DepthFirstSearch(v,x);
					}
				}
			}
		}
	}

	

	bool Automaton::DepthFirstSearch_empty(std::set<int> v,int s )const{
		v.insert(s);
		bool var =true;
		for( auto i : transitions){
			if(i.first.first == s){
				for(int x : i.second){
					if(isStateFinal(x)){
						return false;
					}
					if(v.find(x) == v.end()){
						var =  DepthFirstSearch_empty(v,x);
					}
				}
			}
		}
		return var;
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
		
		for(auto ff : tabStateInit){
			for(auto aa : tabStateFinal){
				if(ff == aa){
					return false;
				}
			}
		}

		if(!init || !final){
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
		
		std::string const fichier1("./img/other.dot");  //On ouvre le fichier
		std::ofstream monFlux1(fichier1.c_str());
  //dot -Tpng figure2.dot -o figure2.png
		other.dotPrint(monFlux1);

	  fa::Automaton mrCuisine = createProduct(*this,other);

		std::string const fichier("./img/mrCuisine.dot");  //On ouvre le fichier
		std::ofstream monFlux(fichier.c_str());
  //dot -Tpng figure2.dot -o figure2.png
		mrCuisine.dotPrint(monFlux);
		return mrCuisine.isLanguageEmpty();
  }

	std::set<int> Automaton::readSymbols(const std::set<int> sete,char a) const{
		std::set<int> res;
		
		for(auto i : sete){
			for(auto st : states){
				if(hasTransition(i,a,st.first)){
					res.insert(st.first);
				}
			}
		}

		return res;
	}

	std::set<int> Automaton::getInitialState() const{
		std::set<int> sI;
		for(auto i : states){
			if(i.second.first){
				sI.insert(i.first);
			}
		}
		return sI;
	}

	std::set<int> Automaton::getFinalState() const{
		std::set<int> sI;
		for(auto i : states){
			if(i.second.second){
				sI.insert(i.first);
			}
		}
		return sI;
	}

  std::set<int> Automaton::readString(const std::string& word) const{
	std::set<int> sI = getInitialState();
	for(auto a : word){
		sI = readSymbols(sI,a);		
	}
    return sI;
  }

  bool Automaton::match(const std::string& word) const{
	std::set<int> res = readString(word);

	for(auto a : states){
		for(auto x : res){
			if(a.first == x){
				if(a.second.second){return true;}
			}
		}
	}

	return false;
  }

  bool Automaton::isIncludedIn(const Automaton& other) const{
	fa::Automaton dyson = createComplement(other);
    return hasEmptyIntersectionWith(dyson);
  }

  Automaton Automaton::createMirror(const Automaton& automaton){
    fa::Automaton bigBrother;
    bigBrother.states=automaton.states;
    bigBrother.alphabet=automaton.alphabet;

    for( auto it : automaton.transitions){
      for(auto i = 0u ; i < it.second.size(); ++i){
        bigBrother.addTransition(it.second[i],it.first.second,it.first.first);
      }
    }

	std::set<int> finale = bigBrother.getFinalState();
	std::set<int> init = bigBrother.getInitialState();

	for(auto a : finale){
		auto i = bigBrother.states.find(a);
		if(!i->second.first){
			i->second.first = true;
			i->second.second = false;
		}
	}

	for(auto a : init){
		auto i = bigBrother.states.find(a);
		if(!i->second.second){
			i->second.first = false;
			i->second.second = true;
		}
	}


    return bigBrother;
  }

	int Automaton::findBinState() const{
		for( auto x : states){
			std::set<int> res;
			for(auto a : transitions){
				for( auto f : alphabet){
					if(a.first.first == x.first && a.first.second == f && !x.second.second){
						for(auto m : a.second){
							res.insert(m);
						}
					}
				}
			}
			if(res.size() == 1 ){
				for (auto g : res){
					if(g == x.first){return g;}
				}
			}
		}


		return -1;
	}

  Automaton Automaton::createComplete(const Automaton& automaton){
    if (automaton.isComplete()){
      return automaton;
    }

    fa::Automaton create = automaton;

    int binState = create.findBinState();
	if(binState < 0){
		binState = 0;
		while(create.hasState(binState)){
			binState++;
		}
		create.addState(binState);
	}
	for(auto a : create.states){
		if(a.first == binState){
			a.second.second = true;
		} 
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
    bool var =true;
	if(!glados.isDeterministic()){glados = createDeterministic(glados);}
    if(!glados.isComplete()){var = false;glados = createComplete(glados);}
	std::set<int> init = glados.getInitialState();
	if(init.size() == 0){
		glados.states.begin()->second.first = true;
	}

    for(auto i = 0u ; i < glados.states.size(); ++i){
      (glados.states[i].second ? glados.states[i].second = false: glados.states[i].second = true );
    }
	if(!var){
		int binState = glados.findBinState();
		for(auto i = 0u ; i < glados.states.size(); ++i){
			if(i == binState){
				glados.states[i].second=true;}
		}
	}
      
    return glados;
  }



  Automaton Automaton::createProduct(const Automaton& lhs, const Automaton& rhs){
		fa::Automaton bosch ;
		std::set_intersection(begin(lhs.alphabet),end(lhs.alphabet),begin(rhs.alphabet),end(rhs.alphabet),inserter(bosch.alphabet,end(bosch.alphabet)));
		
		int compt =  0; 
		std::map<std::pair<int,int>,int> visited;

		for(auto l : lhs.states){
			for(auto r : rhs.states){
				if(l.second.first && r.second.first){
					visited.insert({std::make_pair(l.first,r.first),compt});
					bosch.addState(compt);
					bosch.setStateInitial(compt);
					compt++;
				}
			}
		}

		for(auto bd : visited){
			for(auto a : bosch.alphabet){
				std::set<int> stockG = lhs.readSymbols({bd.first.first},a);
				std::set<int> stockD = rhs.readSymbols({bd.first.second},a);
				for(auto p : stockG){
					for(auto v : stockD){
						auto findKey = visited.find(std::make_pair(p,v));
						if(findKey == visited.end()){
							visited.insert({std::make_pair(p,v),compt});
							bosch.addState(compt);
							bosch.addTransition(bd.second,a,compt);
							compt++;
						}else{
							bosch.addTransition(bd.second,a,findKey->second);
						}
					}
				}
			}
		}

		for(auto bd : visited){
			if(lhs.isStateFinal(bd.first.first) && rhs.isStateFinal(bd.first.second)){
				bosch.setStateFinal(bd.second);
			}
		}
		if(!bosch.countStates()){
			bosch.addState(0);
		}
		if(!bosch.countSymbols()){
			bosch.addSymbol('a');
		}


		return bosch;
  }


  Automaton Automaton::createDeterministic(const Automaton& other){
	fa::Automaton moulinex;
	moulinex.alphabet = other.alphabet;

	std::map<int,std::set<int>> bd;
	int compt = 0;

	bd.insert({compt,other.getInitialState()});
	moulinex.addState(compt);
	moulinex.setStateInitial(compt);
	compt++;

	for(auto bdd : bd){
		for(auto a : moulinex.alphabet){
			std::set<int> stock = other.readSymbols(bdd.second,a);
			bool var = false;
			for(auto key : bd){
				if(key.second == stock){
					moulinex.addTransition(bdd.first,a,key.first);
					var = true;
				}
			}
			if(!var){
				bd.insert({compt,stock});
				moulinex.addState(compt);
				moulinex.addTransition(bdd.first,a,compt);
				compt++;
			}
		}
	}
	
	for(auto bdd :bd){
		for(auto aze : bdd.second){
			if(other.isStateFinal(aze)){
				moulinex.setStateFinal(bdd.first);
				break;
			}
		}
	}


	

	return moulinex;
  }


  Automaton Automaton::createMinimalMoore(const Automaton& other){
    return other;
  }


	Automaton Automaton::createMinimalBrzozowski(const Automaton& other){
		fa::Automaton thermomix = other;
		
		thermomix = createMirror(thermomix);
		
		thermomix = createDeterministic(thermomix);
	
		thermomix = createMirror(thermomix);
		thermomix = createDeterministic(thermomix);
	
		thermomix = createComplete(thermomix);
	
		
		return thermomix;
	}

	

}
