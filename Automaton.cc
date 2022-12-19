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
	  return (!std::isgraph(symbol)) ? false: alphabet.insert(symbol).second;
  }

  bool Automaton::removeSymbol(char symbol){
    if(alphabet.erase(symbol) != 1){
      return false;
    }
	// remove all transition with this symbol
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
	return (state<0) ? false : states.insert({state,std::make_pair(false,false)}).second;
  }

  bool Automaton::removeState(int state){
    if(states.erase(state) != 1){
      return false;
    }
	
	for(auto a : alphabet){
		transitions.erase({state,a});
	}
    // remove all transition who have this state
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
	  // check all specifical case
    if(!hasState(from) || !hasState(to) || hasTransition(from,alpha,to) || (!hasSymbol(alpha) && fa::Epsilon != alpha)){
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
    std::set<int> initState = getInitialState();
    std::set<int> finalState = getFinalState();
	
	// print initial state
    os << "Initial states:\n\t";
    for(int i : initState){
      os << i <<' ';
    } 
	// print final state
    os << "\nFinal states:\n\t"; 
    for(int i : finalState){
      os << i <<' ';
    } 
	// print transitions
    os << "\nTransitions\n";
    int firste = -1 ;
    for (auto i = transitions.begin(); i != transitions.end(); i++) {
      if(i->first.first != firste){
        os << "\tFor state " << i->first.first << ":\n";
      }
      
      
      os << "\t\tFor letter " <<(i->first.second == fa::Epsilon ? '~': i->first.second) << ": ";
      for (size_t x = 0; x < i->second.size(); x++) {
        os << i->second[x] << " ";
      }
      os << std::endl;
      firste = i->first.first;
    }
  }

  void Automaton::dotPrint(std::ostream& os) const {
    os << "digraph Automate {\n\t rankdir=LR\n";
    for (auto i : states) {
        if (isStateInitial(i.first) && isStateFinal(i.first)) {
            os << "\tn" << i.first << "[shape=none label=\"\" height=0 width=0 ]\n\tn" << i.first << "->" << i.first << "\n\t" << i.first << "[shape=doublecircle]\n";
        } else if (isStateInitial(i.first)) {
            os << "\tn" << i.first << "[shape=none label=\"\" height=0 width=0 ]\n\tn" << i.first << "->" << i.first << "\n\t" << i.first << "[shape=circle]\n";
        } else if (isStateFinal(i.first)) {
            os << "\t" << i.first << "[shape=doublecircle]\n";
        }
    }
    os << "\tnode [shape = circle]\n";
    for (auto i : states){
      os << "\t" << i.first << std::endl ;
    }
    for (auto it = transitions.begin(); it != transitions.end(); it++) {
      for (size_t j = 0; j < it->second.size(); j++) {
        os << "\t" << it->first.first << " -> " << it->second[j] << " [label=\"" << (it->first.second == fa::Epsilon ? '~': it->first.second) << "\"]" << std::endl;
      } 
    }
    os << "}\n";
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
    int nbEtatInit = (int)getInitialState().size() ;
    
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
			DepthFirstSearch(false,visited,lo);
		}
		std::set<int> notVisited;
		std::set_difference(begin(r),end(r),begin(visited),end(visited),inserter(notVisited,end(notVisited)));
		
		for(auto fe:notVisited){
			removeState(fe);
		}

	}

	void Automaton::removeNonCoAccessibleStates(){
		// mirror removeNonAccessibleStates and mirror remove all NonCoAccessiblesState
		*this=createMirror(*this);
		removeNonAccessibleStates();
		*this=createMirror(*this);
	}
	
	bool Automaton::DepthFirstSearch(bool emptyChoice,std::set<int>& v,int s )const{
		v.insert(s);
		bool var = true;
		for( auto i : transitions){
			if(i.first.first == s){
				for(int x : i.second){
					if(emptyChoice){
						if(isStateFinal(x)){
							return false;
						}
						if(var && v.find(x) == v.end()){
						
							var = DepthFirstSearch(true,v,x);
						}

					}else{
						if(v.find(x) == v.end()){
							DepthFirstSearch(false,v,x);
						}
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
		// check if one state is initial and final
		if(!init || !final){
			return true;
		}
		
		std::set<int> v;
		for(auto x : tabStateInit){
			if(!DepthFirstSearch(true,v,x)){
				return false;
			}	
		}
		
		return true;
	}

  bool Automaton::hasEmptyIntersectionWith(const Automaton& other) const{
	  return createProduct(*this,other).isLanguageEmpty();
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

  bool Automaton::isIncludedIn(const Automaton& other) const {
    fa::Automaton CleanMate = other;
    for(auto alpha : alphabet) {
      if(!CleanMate.hasSymbol(alpha)) {
        CleanMate.addSymbol(alpha);
      }
    }
	
    return hasEmptyIntersectionWith(fa::Automaton::createComplement(CleanMate));
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
	if(!glados.isDeterministic()){glados = createDeterministic(glados);}
    if(!glados.isComplete()){glados = createComplete(glados);}
	for(auto &sta : glados.states){(sta.second.second) ? sta.second.second = false: sta.second.second = true;}
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
	if(other.isDeterministic()){return other;}
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

  std::vector<std::pair<char,std::vector<int>>> Automaton::rempliTableauMoor(const Automaton& copy,std::vector<std::pair<char,std::vector<int>>> n,std::vector<int> sta){
	for(char a : copy.alphabet){
		  std::vector<int> ligne;
		  for(int cmp = 0 ;  cmp < (int)sta.size() ; ++cmp){
			  auto fi = copy.transitions.find({sta[cmp],a});
			  int cmp2 = 0;
			  for(auto edf : sta){
				  if(edf == fi->second[0]){
					  ligne.push_back(n[0].second[cmp2]);
				  }
				  cmp2++;
			  }
		  }
		  
		  n.push_back(std::make_pair(a,ligne));
	  }
	return n;
  }

	std::vector<int> Automaton::returnLigne0Tab(std::vector<std::pair<char,std::vector<int>>> tabAvant){
		std::vector<int> res;
		std::vector<std::vector<int>> tabtab;
		size_t size = tabAvant[0].second.size();
		for(size_t i = 0 ; i < size;++i){
			std::vector<int> column;
			for(auto a :tabAvant){
				for(size_t e = 0; e < a.second.size();e++){
					if(e == i){
						column.push_back(a.second[e]);
					}
				}
			}
			tabtab.push_back(column);
		}
	
		std::vector<std::pair<int,std::vector<int>>> column;
		int compt = 1;
		for(auto a : tabtab){
			if(column.size() == 0){
				res.push_back(1);
				column.push_back({compt,a});
				compt++;
			}else{
				bool var = true;
				for(auto c : column){
					if(a == c.second){
						res.push_back(c.first);
						var = false;
					}
				}
				if(var){
					res.push_back(compt);
					column.push_back({compt,a});
			 		compt++;
				}
			}

		}
		
		return res;
	}

	void Automaton::printMinimalMooreStepTab(std::vector<std::pair<char,std::vector<int>>> nZero,int& count){
		std::cout << std::endl;
	  for(auto a : nZero){
		  std::cout << "|";
		  if(a.first == '~'){
			  std::cout << a.first << count;
			  count++;
		  }else{
			  std::cout << a.first << " ";
		  }
		  std::cout << "|";
		  for(auto e : a.second){
			  std::cout << e << "|";
		  }
		  std::cout << std::endl;
	  }
	  std::cout << std::endl;

	}

  Automaton Automaton::createMinimalMoore(const Automaton& other){
	  fa::Automaton copy =  other;
	  copy.removeNonAccessibleStates();
	  copy = createComplete(copy);
	  copy = createDeterministic(copy);
	  fa::Automaton R2D2;
	  R2D2.alphabet = copy.alphabet;
	  
	  std::vector<int> sta;
	  std::vector<std::pair<char,std::vector<int>>> nZero;
	  std::vector<std::pair<char,std::vector<int>>> nPlusUn;
		
	  std::vector<int> res;
	  for(auto e : copy.states){	  
		  sta.push_back(e.first);
		  if(copy.isStateFinal(e.first)){
			  res.push_back(2);
		  }else{
			  res.push_back(1);
		  }
	  }

	  nZero.push_back(std::make_pair('~',res));
	  nZero=  rempliTableauMoor(copy,nZero,sta);
	  nPlusUn.push_back(std::make_pair('~',returnLigne0Tab(nZero)));
	  nPlusUn=  rempliTableauMoor(copy,nPlusUn,sta);


	  while(nZero[0].second != nPlusUn[0].second){
		  nZero=nPlusUn;
		  std::vector<std::pair<char,std::vector<int>>> newe;
		  newe.push_back(std::make_pair('~',returnLigne0Tab(nZero)));
		  newe =  rempliTableauMoor(copy,newe,sta);
		  nPlusUn = newe;
	  }


	for(int i = 0 ; i < (int)nPlusUn[0].second.size();++i){
			R2D2.addState(nPlusUn[0].second[i]);
			if(copy.isStateInitial(sta[i])){
				R2D2.setStateInitial(nPlusUn[0].second[i]);
			}
			if(copy.isStateFinal(sta[i])){
				R2D2.setStateFinal(nPlusUn[0].second[i]);
			}
	}
	
	for(auto ligne : nPlusUn){
		if(ligne.first == '~'){continue;}
		for(int i = 0 ; i < (int)ligne.second.size();++i){
			R2D2.addTransition(nPlusUn[0].second[i],ligne.first,ligne.second[i]);
		}
	}
	if(!R2D2.getInitialState().size()){
		for(auto zaz : R2D2.states){
			R2D2.setStateInitial(zaz.first);
			break;
		}
	}
	  return R2D2;
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
