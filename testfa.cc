#include "gtest/gtest.h"

#include "Automaton.h"
#include <fstream>
#include <iostream>

#define BIG_SIZE 1000

/**
  Private method generateAutomaton
*/
void createAutomaton(fa::Automaton& mamiRobot,int nbState, std::vector<char> diction){
  for(int i = 0 ; i < nbState ; ++i){
    EXPECT_TRUE(mamiRobot.addState(i));
    EXPECT_TRUE(mamiRobot.hasState(i));
  }
  for(size_t i = 0 ; i < diction.size() ; ++i){
    EXPECT_TRUE(mamiRobot.addSymbol(diction[i]));
    EXPECT_TRUE(mamiRobot.hasSymbol(diction[i]));
  }
}




/* void testDotPrint(const fa::Automaton& mamiRobot,std::string file){ */
/*   std::string const fichier("./img/"+file+".dot");  //On ouvre le fichier */
/*   std::ofstream monFlux(fichier.c_str()); */
/*   //dot -Tpng figure2.dot -o figure2.png */
/*   mamiRobot.dotPrint(monFlux); */
/* } */

TEST(createComplete, test) {
  fa::Automaton fa;
  createAutomaton(fa,4,{'a','b'});
  fa.setStateInitial(0);
  fa.addTransition(0,'a',1);
  fa.addTransition(1,'a',0);
  /* testDotPrint(fa,"test"); */
  EXPECT_TRUE(fa.isDeterministic());
}


/*
 * Automaton constructor
 */

TEST(Automaton, Empty) {
  fa::Automaton fa;
  EXPECT_FALSE(fa.isValid());
}

/*
 * isValid
 */

TEST(isValid, ValidAutomate) {
  fa::Automaton fa;
  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,1,tab);
  EXPECT_TRUE(fa.isValid());
}

TEST(isValid, OneStateNoSymbol) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(1));
  static const std::vector<char> tab = {};
  createAutomaton(fa,1,tab);
  EXPECT_FALSE(fa.isValid());
}

TEST(isValid, NoStateOneSymbol) {
  fa::Automaton fa;
  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,0,tab);
  EXPECT_FALSE(fa.isValid());
}



/*
 * addSymbol
 */
TEST(addSymbol, OneSymbol) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addSymbol('a'));
  
}

TEST(addSymbol, ManySymbols) {
  fa::Automaton fa;
  static const std::vector<char> tab = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i' };
  createAutomaton(fa,0,tab);
  EXPECT_EQ(tab.size(),fa.countSymbols());
}

TEST(addSymbol, Epsilon) {
  fa::Automaton fa;
  EXPECT_FALSE(fa.addSymbol(fa::Epsilon));
}

TEST(addSymbol, NoValideSymbol) {
  fa::Automaton fa;
  EXPECT_FALSE(fa.addSymbol('\t'));
}



/*
 * removeSymbol
 */
TEST(removeSymbol, OneSymbol) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addSymbol('a'));
  EXPECT_TRUE(fa.removeSymbol('a'));
  EXPECT_EQ(0,(int)fa.countSymbols());
}

TEST(removeSymbol, ManySymbols) {
  fa::Automaton fa;
  static const std::vector<char> tab = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i' };

  for (size_t i = 0; i < tab.size(); ++i) {
    EXPECT_TRUE(fa.addSymbol(tab[i]));
  }
  EXPECT_TRUE(fa.removeSymbol('a'));
  EXPECT_TRUE(fa.removeSymbol('b'));
  EXPECT_EQ(tab.size()-2,fa.countSymbols());
}

TEST(removeSymbol, InvalidSymbol) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addSymbol('b'));
  EXPECT_FALSE(fa.removeSymbol('a'));
  EXPECT_EQ(1,(int)fa.countSymbols());
}

/*
 * hasSymbol
 */
TEST(hasSymbol, PresentSymbol) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addSymbol('a'));
  EXPECT_TRUE(fa.hasSymbol('a'));
}

TEST(hasSymbol, NoPresentSymbol) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addSymbol('a'));
  EXPECT_FALSE(fa.hasSymbol('b'));
}





/*
 * addState
 */
TEST(addState, OneState) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(1));
  EXPECT_FALSE(fa.isStateInitial(1));
  EXPECT_FALSE(fa.isStateFinal(1));
}
 
TEST(addState, ManyState) {
  fa::Automaton fa;

  for (int i = 0; i < BIG_SIZE; ++i) {
    EXPECT_TRUE(fa.addState(i));
    EXPECT_FALSE(fa.isStateInitial(i));
    EXPECT_FALSE(fa.isStateFinal(i));
  }
  EXPECT_EQ(BIG_SIZE,(int)fa.countStates());
}

TEST(addState, StateALreadyExist) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(1));
  EXPECT_FALSE(fa.isStateInitial(1));
  EXPECT_FALSE(fa.isStateFinal(1));
  EXPECT_FALSE(fa.addState(1));
}

TEST(addState, NegativState) {
  fa::Automaton fa;
  EXPECT_FALSE(fa.addState(-1));
}

/*
 * removeState
 */
TEST(removeState, OneSymbol) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(1));
  EXPECT_TRUE(fa.removeState(1));
  EXPECT_EQ(0,(int)fa.countStates());
}

TEST(removeState, ManySymbols) {
  fa::Automaton fa;
  for (int i = 0; i < BIG_SIZE; ++i) {
    EXPECT_TRUE(fa.addState(i));
  }
  EXPECT_TRUE(fa.removeState(1));
  EXPECT_TRUE(fa.removeState(666));
  EXPECT_EQ(BIG_SIZE-2,(int)fa.countStates());
}

TEST(removeState, InvalidSymbol) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(1));
  EXPECT_FALSE(fa.removeState(2));
  EXPECT_EQ(1,(int)fa.countStates());
}

/*
 * hasState
 */
TEST(hasState, PresentState) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(1));
  EXPECT_TRUE(fa.hasState(1));
}

TEST(hasState, NoPresentState) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(1));
  EXPECT_FALSE(fa.hasState(2));
}

/*
 * setStateInitial
 */
TEST(setStateInitial, Valid) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(1));
  fa.setStateInitial(1);
  EXPECT_TRUE(fa.isStateInitial(1));
}

TEST(setStateInitial, noInitial) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(1));
  EXPECT_FALSE(fa.isStateInitial(1));
}

TEST(setStateInitial, NoStateIndex) {
  fa::Automaton fa;
  fa.setStateInitial(1);
}

TEST(setStateInitial, AlreadyInitial) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(1));
  fa.setStateInitial(1);
  EXPECT_TRUE(fa.isStateInitial(1));
  fa.setStateInitial(1);
  EXPECT_TRUE(fa.isStateInitial(1));
}

TEST(setStateInitial, ReadEmptyString) {
	fa::Automaton fa;
	std::vector<char> tab= {};
	createAutomaton(fa,150,tab);
	
	fa.setStateInitial(7);
	fa.setStateInitial(13);
	fa.setStateInitial(19);


	EXPECT_TRUE(fa.isStateInitial(7));
	EXPECT_TRUE(fa.isStateInitial(13));
	EXPECT_TRUE(fa.isStateInitial(19));
	

}


/*
 * setStateFinal
 */
TEST(setStateFinal, Valid) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(1));
  fa.setStateFinal(1);
  EXPECT_TRUE(fa.isStateFinal(1));
}

TEST(setStateFinal, NoStateIndex) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(1));
  EXPECT_FALSE(fa.isStateFinal(0));
}

TEST(setStateFinal, AlreadyFinal) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(1));
  fa.setStateFinal(1);
  EXPECT_TRUE(fa.isStateFinal(1));
  fa.setStateFinal(1);
  EXPECT_TRUE(fa.isStateFinal(1));
}

TEST(setStateFinal, FinalInitial) {
  fa::Automaton fa;
  EXPECT_TRUE(fa.addState(1));
  fa.setStateFinal(1);
  EXPECT_TRUE(fa.isStateFinal(1));
  fa.setStateInitial(1);
  EXPECT_TRUE(fa.isStateInitial(1));
}


 /*
  * addTransition
  */
TEST(addTransition, OneElement) {
  fa::Automaton fa;

  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,2,tab);
  EXPECT_TRUE(fa.addTransition(0,'a',1));
  EXPECT_TRUE(fa.hasTransition(0,'a',1));
}

TEST(addTransition, EpsilonTransition) {
  fa::Automaton fa;
  static const std::vector<char> tab = {};
  createAutomaton(fa,2,tab);
  EXPECT_TRUE(fa.addTransition(0,fa::Epsilon,1));
}

TEST(addTransition, AlreadyExist) {
  fa::Automaton fa;
  
  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,2,tab);

  EXPECT_TRUE(fa.addTransition(0,'a',1));
  EXPECT_FALSE(fa.addTransition(0,'a',1));
  EXPECT_TRUE(fa.hasTransition(0,'a',1));
  EXPECT_EQ(1,(int)fa.countTransitions());
}

TEST(addTransition, SymbolNotExist) {
  fa::Automaton fa;

  static const std::vector<char> tab = {};
  createAutomaton(fa,2,tab);

  EXPECT_FALSE(fa.addTransition(0,'a',1));
  EXPECT_FALSE(fa.hasTransition(0,'a',1));
}

TEST(addTransition, FirstStateNotExist) {
  fa::Automaton fa;
  
  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,1,tab);

  EXPECT_FALSE(fa.addTransition(1,'a',0));
  EXPECT_FALSE(fa.hasTransition(1,'a',0));
}

TEST(addTransition, SecondStateNotExist) {
  fa::Automaton fa;

  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,1,tab);

  EXPECT_FALSE(fa.addTransition(0,'a',1));
  EXPECT_FALSE(fa.hasTransition(0,'a',1));
}

/*
 * Remove a transition
 */
TEST(removeTransition, OneElement) {
  fa::Automaton fa;
  
  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,2,tab);

  EXPECT_TRUE(fa.addTransition(0,'a',1));
  EXPECT_TRUE(fa.hasTransition(0,'a',1));
  EXPECT_TRUE(fa.removeTransition(0,'a',1));
  EXPECT_FALSE(fa.hasTransition(0,'a',1));
}

TEST(removeTransition, SymbolNotExist) {
  fa::Automaton fa;

  static const std::vector<char> tab = {};
  createAutomaton(fa,2,tab);

  EXPECT_FALSE(fa.removeTransition(0,'a',1));
}

TEST(removeTransition, FirstStateNotExist) {
  fa::Automaton fa;
  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,1,tab);
  EXPECT_FALSE(fa.removeTransition(1,'a',0));
}

TEST(removeTransition, SecondStateNotExist) {
  fa::Automaton fa;
  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,1,tab);
  EXPECT_FALSE(fa.removeTransition(0,'a',1));
}

TEST(removeTransition, ByTheRemovedOfSymbol) {
  fa::Automaton fa;

  static const std::vector<char> tab = {'a','b'};
  createAutomaton(fa,2,tab);

  EXPECT_TRUE(fa.addTransition(0,'b',1));
  EXPECT_TRUE(fa.addTransition(0,'a',1));
  EXPECT_TRUE(fa.removeSymbol('a'));  
  EXPECT_FALSE(fa.hasTransition(0,'a',1));
  EXPECT_TRUE(fa.hasTransition(0,'b',1));
}

TEST(removeTransition, ByTheRemovedFirstState) {
  fa::Automaton fa;

  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,2,tab);

  EXPECT_TRUE(fa.addTransition(0,'a',1));
  EXPECT_TRUE(fa.removeState(0));  
  EXPECT_FALSE(fa.hasTransition(0,'a',1));
}

TEST(removeTransition, ByTheRemovedSecondState) {
  fa::Automaton fa;

  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,2,tab);

  EXPECT_TRUE(fa.addTransition(0,'a',1));
  EXPECT_TRUE(fa.removeState(1));  
  EXPECT_FALSE(fa.hasTransition(0,'a',1));
}

/*
 * hasTransition
 */
TEST(removeTransition, Valid) {
  fa::Automaton fa;

  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,2,tab);
  EXPECT_TRUE(fa.addTransition(0,'a',1));
  EXPECT_TRUE(fa.hasTransition(0,'a',1));
  EXPECT_TRUE(fa.hasTransition(0,'a',1));
}



/*
 * hasEpsilonTransition
 */
TEST(hasEpsilonTransition, OneValide) {
  fa::Automaton fa;

  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,2,tab);

  EXPECT_TRUE(fa.addTransition(0,fa::Epsilon,1));
  EXPECT_TRUE(fa.isValid());
  EXPECT_TRUE(fa.hasEpsilonTransition());
}

TEST(hasEpsilonTransition, NotIn) {
  fa::Automaton fa;
  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,2,tab);

  EXPECT_TRUE(fa.addTransition(0,'a',1));
  EXPECT_FALSE(fa.hasEpsilonTransition());
}


/*
 * isDeterministic
 */
TEST(isDeterministic, Valid) {
  fa::Automaton fa;
  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,2,tab);
  fa.setStateInitial(0);
  
  EXPECT_TRUE(fa.isValid());
  EXPECT_TRUE(fa.isDeterministic());
}

TEST(isDeterministic, NoInitialState) {
  fa::Automaton fa;
  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,2,tab);  

  EXPECT_TRUE(fa.isValid());
  EXPECT_FALSE(fa.isDeterministic());
}

TEST(isDeterministic, TooManyInitialState) {
  fa::Automaton fa;

  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,2,tab);

  fa.setStateInitial(0);
  fa.setStateInitial(1);
  
  
  EXPECT_TRUE(fa.isValid());
  EXPECT_FALSE(fa.isDeterministic());
}

TEST(isDeterministic, TransitionError) {
  fa::Automaton fa;

  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,3,tab);
  fa.setStateInitial(0);


  EXPECT_TRUE(fa.addTransition(1,'a',1));
  EXPECT_TRUE(fa.addTransition(1,'a',2));
  
  EXPECT_TRUE(fa.isValid());
  EXPECT_FALSE(fa.isDeterministic());
}



/*
 * isComplete
 */
TEST(isComplete, Valid) {
  fa::Automaton fa;

  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,2,tab);

  EXPECT_TRUE(fa.addTransition(0,'a',0));
  EXPECT_TRUE(fa.addTransition(0,'a',1));
  EXPECT_TRUE(fa.addTransition(1,'a',1));
  
  EXPECT_TRUE(fa.isValid());
  EXPECT_TRUE(fa.isComplete());
}


TEST(isComplete, NoValid) {
  fa::Automaton fa;

  
  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,2,tab);

  EXPECT_TRUE(fa.addTransition(0,'a',0));
  EXPECT_TRUE(fa.addTransition(0,'a',1));
  
  EXPECT_TRUE(fa.isValid());
  EXPECT_FALSE(fa.isComplete());
}

TEST(isComplete, NoTransition) {
  fa::Automaton fa;

  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,2,tab);

  EXPECT_TRUE(fa.addTransition(0,'a',0));
  EXPECT_TRUE(fa.addTransition(0,'a',1));
  
  EXPECT_TRUE(fa.isValid());
  EXPECT_FALSE(fa.isComplete());
}

TEST(isComplete, AddedRemovedTransition){
	fa:: Automaton fa;

	static const std::vector<char> tab = {'a'};
	createAutomaton(fa,1,tab);

	EXPECT_TRUE(fa.addTransition(0,'a',0));
	EXPECT_TRUE(fa.removeTransition(0,'a',0));
	
	EXPECT_FALSE(fa.isComplete());

}

/**
  createComplete
*/
TEST(createComplete, alreadyComplet) {
  fa::Automaton fa;

  static const std::vector<char> tab = {'a'};
  createAutomaton(fa,2,tab);

  EXPECT_TRUE(fa.addTransition(0,'a',0));
  EXPECT_TRUE(fa.addTransition(0,'a',1));
  EXPECT_TRUE(fa.addTransition(1,'a',1));
  
  EXPECT_TRUE(fa.isValid());
  EXPECT_TRUE(fa.isComplete());
  fa = fa.createComplete(fa);

  EXPECT_TRUE(fa.isValid());
  EXPECT_TRUE(fa.isComplete());
}

TEST(createComplete, noComplet) {
  fa::Automaton fa;

  static const std::vector<char> tab = {'a','b'};
  createAutomaton(fa,4,tab);

  fa.setStateInitial(3);
  fa.setStateFinal(2);

  EXPECT_TRUE(fa.addTransition(3,'b',0));
  EXPECT_TRUE(fa.addTransition(0,'a',1));
  EXPECT_TRUE(fa.addTransition(1,'a',1));
  EXPECT_TRUE(fa.addTransition(1,'b',2));

  EXPECT_TRUE(fa.isValid());
  EXPECT_FALSE(fa.isComplete());

  
  fa = fa.createComplete(fa);

  EXPECT_TRUE(fa.isValid());
  EXPECT_TRUE(fa.isComplete());

  /* testDotPrint(fa,"figure4"); */

}


/**
  create a complement automate 
*/
TEST(createComplement, valid) {
  fa::Automaton fa;
  static const std::vector<char> tab = {'a','b'};
  createAutomaton(fa,3,tab);

  fa.setStateInitial(0);
  fa.setStateFinal(1);

  EXPECT_TRUE(fa.addTransition(0,'a',0));
  EXPECT_TRUE(fa.addTransition(0,'b',1));
  EXPECT_TRUE(fa.addTransition(1,'b',1));
  EXPECT_TRUE(fa.addTransition(1,'a',2));
  EXPECT_TRUE(fa.addTransition(2,'a',2));
  EXPECT_TRUE(fa.addTransition(2,'b',2));

  EXPECT_TRUE(fa.isValid());
  EXPECT_TRUE(fa.isComplete());
  EXPECT_TRUE(fa.isDeterministic());

  std::string motReconnus = "aaabb";
  EXPECT_TRUE(fa.match(motReconnus));
  
  fa = fa.createComplement(fa);
  
  EXPECT_TRUE(fa.isValid());
  EXPECT_TRUE(fa.isComplete());
  EXPECT_TRUE(fa.isDeterministic());

  EXPECT_EQ(2u,fa.countSymbols());
  EXPECT_TRUE(fa.hasSymbol('a'));
  EXPECT_TRUE(fa.hasSymbol('b'));
  EXPECT_FALSE(fa.match(motReconnus));
  EXPECT_TRUE(fa.match("bbbba"));
  //testDotPrint(fa,"complement");

}


TEST(createComplement, noCompletNoDeterministic){
	fa::Automaton fa;

	static const std::vector<char> tab = {'a','b','c'};
	createAutomaton(fa,5,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(3);
	
	EXPECT_TRUE(fa.addTransition(0,'b',1));
	EXPECT_TRUE(fa.addTransition(0,'a',2));
	EXPECT_TRUE(fa.addTransition(1,'c',3));
	EXPECT_TRUE(fa.addTransition(2,'a',2));
	EXPECT_TRUE(fa.addTransition(4,'a',2));

	EXPECT_EQ(3,fa.countSymbols());
	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.isComplete());

	EXPECT_FALSE(fa.match("c"));
	EXPECT_FALSE(fa.match("bac"));
	
	fa = fa.createComplement(fa);
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(3,fa.countSymbols());
	EXPECT_TRUE(fa.isComplete());
	
	EXPECT_TRUE(fa.match("c"));
	EXPECT_TRUE(fa.match("bac"));
}

TEST(createComplement, noInitialState){
	fa::Automaton fa;

	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,1,tab);
	
	EXPECT_TRUE(fa.addTransition(0,'a',0));
	fa.setStateFinal(0);

	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.isComplete());

	EXPECT_TRUE(fa.isLanguageEmpty());
	fa = fa.createComplement(fa);
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isComplete());

	EXPECT_FALSE(fa.isLanguageEmpty());
	EXPECT_TRUE(fa.match("aaaaaaaaaaaaaa"));
}

TEST(createComplement,noFinalState){
	fa::Automaton fa;

	static const std::vector<char> tab = {'a','b','c'};
	createAutomaton(fa,5,tab);
	
	fa.setStateInitial(0);
	
	EXPECT_TRUE(fa.addTransition(0,'b',1));
	EXPECT_TRUE(fa.addTransition(0,'a',2));
	EXPECT_TRUE(fa.addTransition(1,'c',3));
	EXPECT_TRUE(fa.addTransition(2,'a',2));
	EXPECT_TRUE(fa.addTransition(4,'a',2));

	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.isComplete());
	EXPECT_TRUE(fa.isLanguageEmpty());
	
	fa = fa.createComplement(fa);

	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.isLanguageEmpty());
	EXPECT_TRUE(fa.isComplete());

}

TEST(createComplement,Complement2Times){
	fa::Automaton fa;

	static const std::vector<char> tab = {'a','b','c'};
	createAutomaton(fa,5,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(3);
	
	EXPECT_TRUE(fa.addTransition(0,'b',1));
	EXPECT_TRUE(fa.addTransition(0,'a',2));
	EXPECT_TRUE(fa.addTransition(1,'c',3));
	EXPECT_TRUE(fa.addTransition(2,'a',2));
	EXPECT_TRUE(fa.addTransition(4,'a',2));

	EXPECT_EQ(3,fa.countSymbols());
	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.isComplete());

	EXPECT_FALSE(fa.match("c"));
	EXPECT_FALSE(fa.match("bac"));
	
	fa = fa.createComplement(fa);
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(3,fa.countSymbols());
	EXPECT_TRUE(fa.isComplete());
	
	EXPECT_TRUE(fa.match("c"));
	EXPECT_TRUE(fa.match("bac"));
	
	fa = fa.createComplement(fa);
		
	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(3,fa.countSymbols());
	EXPECT_TRUE(fa.isComplete());

	EXPECT_FALSE(fa.match("c"));
	EXPECT_FALSE(fa.match("bac"));
}




/**
  create a mirror automate 
*/
TEST(createMirror, valid) {
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);

	fa.setStateInitial(0);
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(0,'a',2));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.addTransition(2,'b',2));
	EXPECT_TRUE(fa.addTransition(2,'b',0));
	
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(5,fa.countTransitions());
	EXPECT_EQ(2,fa.countSymbols());
	EXPECT_FALSE(fa.match("bab"));
	EXPECT_TRUE(fa.match("aba"));
	EXPECT_TRUE(fa.match("a"));
	EXPECT_FALSE(fa.match("ba"));
	
	fa = fa.createMirror(fa);
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.match("bab"));
	EXPECT_TRUE(fa.match("aba"));
	EXPECT_TRUE(fa.match("ba"));
	EXPECT_TRUE(fa.match("a"));
	EXPECT_EQ(5,fa.countTransitions());
	EXPECT_EQ(2,fa.countSymbols());

  /* testDotPrint(fa,"mirror2"); */
}

TEST(createMirror, NoTransition){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);

	fa.setStateInitial(0);
	fa.setStateFinal(2);
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(0,fa.countTransitions());
	EXPECT_EQ(2,fa.countSymbols());
	EXPECT_TRUE(fa.isLanguageEmpty());

	fa = fa.createMirror(fa);

	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(0,fa.countTransitions());
	EXPECT_EQ(2,fa.countSymbols());
	EXPECT_TRUE(fa.isLanguageEmpty());
}

TEST(createMirror, stateInitioFinal){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,1,tab);

	fa.setStateInitial(0);
	fa.setStateFinal(0);

	EXPECT_TRUE(fa.addTransition(0,'a',0));
	
	EXPECT_TRUE(fa.match("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(2,fa.countSymbols());
	EXPECT_EQ(1,fa.countStates());
	EXPECT_EQ(1,fa.countTransitions());
	
	fa = fa.createMirror(fa);
	

	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(2,fa.countSymbols());
	EXPECT_EQ(1,fa.countStates());
	EXPECT_EQ(1,fa.countTransitions());
	EXPECT_TRUE(fa.match("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
	
}

/**
 * isLanguageEmpty
 */ 
TEST(isLanguageEmpty, validNoInitialState){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,5,tab);
	
	fa.setStateFinal(0);
	EXPECT_TRUE(fa.addTransition(0,'a',2));
	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isLanguageEmpty());
}

TEST(isLanguageEmpty, validNoFinalState){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,5,tab);
	
	fa.setStateInitial(0);
	EXPECT_TRUE(fa.addTransition(0,'a',3));
	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isLanguageEmpty());
}

TEST(isLanguageEmpty, validNoTransitionToFinalState){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,5,tab);
	
	fa.setStateFinal(1);
	fa.setStateInitial(0);
	EXPECT_TRUE(fa.addTransition(0,'a',3));
	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isLanguageEmpty());
}

TEST(isLanguageEmpty, noValid){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,5,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(1);
	EXPECT_TRUE(fa.addTransition(0,'a',0));
	EXPECT_TRUE(fa.addTransition(0,'b',1));
	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.isLanguageEmpty());
}

TEST(isLanguageEmpty, noValidAutomateExem){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,8,tab);
	
	fa.setStateInitial(0);
	fa.setStateInitial(2);
	fa.setStateFinal(4);
	fa.setStateFinal(5);
	
	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.addTransition(2,'a',4));
	EXPECT_TRUE(fa.addTransition(3,'a',5));
	EXPECT_TRUE(fa.addTransition(3,'b',6));
	EXPECT_TRUE(fa.addTransition(4,'a',3));
	EXPECT_TRUE(fa.addTransition(4,'a',7));
	EXPECT_TRUE(fa.addTransition(4,'b',2));
	EXPECT_TRUE(fa.addTransition(4,'b',7));
	EXPECT_TRUE(fa.addTransition(5,'a',1));
	EXPECT_TRUE(fa.addTransition(7,'a',4));

	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.isLanguageEmpty());
}

TEST(isLanguageEmpty, initialStateIsFinal){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b','c'};
	createAutomaton(fa,3,tab);

	fa.setStateInitial(0);
	fa.setStateFinal(0);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',1));
	EXPECT_TRUE(fa.addTransition(1,'c',2));
	EXPECT_TRUE(fa.addTransition(1,'c',0));
	EXPECT_TRUE(fa.addTransition(2,'a',0));

	EXPECT_TRUE(fa.isValid());

	EXPECT_FALSE(fa.isLanguageEmpty());
}

TEST(isLanguageEmpty, EmptyWord){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b','c'};
	createAutomaton(fa,3,tab);

	fa.setStateInitial(0);
	fa.setStateFinal(0);

	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.isLanguageEmpty());

}


/*
 * removeNonAccessibleStates
 */
TEST(removeNonAccessibleStates,NoInitialState){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b','c'};
	createAutomaton(fa,10,tab);
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isLanguageEmpty());
	EXPECT_EQ(10,fa.countStates());

	fa.removeNonAccessibleStates();
	
	EXPECT_EQ(1,fa.countStates());
	EXPECT_TRUE(fa.isLanguageEmpty());
	EXPECT_TRUE(fa.isValid());
}

TEST(removeNonAccessibleStates, noNonAccessiblesStates){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b','c'};
	createAutomaton(fa,4,tab);

	fa.setStateInitial(0);
	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'a',2));
	EXPECT_TRUE(fa.addTransition(2,'a',3));

	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(4,fa.countStates());

	fa.removeNonAccessibleStates();

	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(4,fa.countStates());


}


TEST(removeNonAccessibleStates, lastNoAccessible){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b','c'};
	createAutomaton(fa,4,tab);

	fa.setStateInitial(0);
	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'a',2));
	EXPECT_TRUE(fa.addTransition(3,'a',3));

	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(4,fa.countStates());
	EXPECT_EQ(3,fa.countTransitions());

	fa.removeNonAccessibleStates();

	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.hasState(3));
	EXPECT_EQ(3,fa.countStates());
	EXPECT_EQ(2,fa.countTransitions());


}

TEST(removeNonAccessibleStates, chained){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b','c'};
	createAutomaton(fa,4,tab);
	
	fa.setStateInitial(0);
	
	EXPECT_TRUE(fa.addTransition(1,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'a',2));
	EXPECT_TRUE(fa.addTransition(2,'a',3));
	EXPECT_TRUE(fa.addTransition(3,'a',3));

	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(4,fa.countStates());
	EXPECT_EQ(4,fa.countTransitions());

	fa.removeNonAccessibleStates();


	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(1,fa.countStates());
	EXPECT_EQ(0,fa.countTransitions());

}


/*
 * removeNonCoAccessibleStates
 */
TEST(removeNonCoAccessibleStates,noFinalState){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b','c'};
	createAutomaton(fa,4,tab);
	
	fa.setStateInitial(0);
	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'a',2));
	EXPECT_TRUE(fa.addTransition(2,'a',3));
	EXPECT_TRUE(fa.addTransition(3,'a',3));

	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(4,fa.countTransitions());
	EXPECT_EQ(4,fa.countStates());

	fa.removeNonCoAccessibleStates();
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(0,fa.countTransitions());
	EXPECT_EQ(1,fa.countStates());
}


TEST(removeNonCoAccessibleStates,noNonCoAccessiblesState){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b','c'};
	createAutomaton(fa,4,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(3);
	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'a',2));
	EXPECT_TRUE(fa.addTransition(2,'a',3));
	EXPECT_TRUE(fa.addTransition(3,'a',3));

	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(4,fa.countTransitions());
	EXPECT_EQ(4,fa.countStates());

	fa.removeNonCoAccessibleStates();
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(4,fa.countTransitions());
	EXPECT_EQ(4,fa.countStates());
}

TEST(removeNonCoAccessibleStates,firstStateNoCoAccessible){

	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b','c'};
	createAutomaton(fa,4,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(3);

	EXPECT_TRUE(fa.addTransition(0,'a',0));
	EXPECT_TRUE(fa.addTransition(1,'a',2));
	EXPECT_TRUE(fa.addTransition(2,'a',3));
	EXPECT_TRUE(fa.addTransition(3,'a',3));
	
	EXPECT_TRUE(fa.isValid());

	fa.removeNonCoAccessibleStates();

	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.hasState(0));
	EXPECT_EQ(3,fa.countStates());
}




/*
 *	createProduct
 */
TEST(createProduct,sameAutomate){
	fa::Automaton lhs;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(lhs,2,tab);
	
	lhs.setStateInitial(0);
	lhs.setStateFinal(1);
	
	EXPECT_TRUE(lhs.addTransition(0,'a',1));
	EXPECT_TRUE(lhs.addTransition(0,'b',1));
	EXPECT_TRUE(lhs.addTransition(1,'a',1));
	EXPECT_TRUE(lhs.isValid());

	fa::Automaton rhs = lhs;
	fa::Automaton fa = fa.createProduct(lhs,rhs);
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.isLanguageEmpty());
	EXPECT_TRUE(fa.match("b"));
	EXPECT_TRUE(fa.match("a"));
	EXPECT_TRUE(fa.match("baaaaaaaa"));
	EXPECT_TRUE(fa.match("aaaaaaaaaa"));
	EXPECT_FALSE(fa.match("abbbbbbbbbbbbbbbbbbbb"));

}

TEST(createProduct,EmptyProduct){
	fa::Automaton lhs;
	static const std::vector<char> tab = {'p','q'};
	createAutomaton(lhs,3,tab);
	
	lhs.setStateInitial(0);
	lhs.setStateFinal(2);

	EXPECT_TRUE(lhs.addTransition(0,'p',1));
	EXPECT_TRUE(lhs.addTransition(0,'p',2));

	EXPECT_TRUE(lhs.isValid());
	
	fa::Automaton rhs;
	static const std::vector<char> tabe = {'h','y'};
	createAutomaton(rhs,3,tabe);

	EXPECT_TRUE(rhs.addTransition(0,'h',0));
	EXPECT_TRUE(rhs.addTransition(0,'h',1));
	EXPECT_TRUE(rhs.addTransition(1,'y',2));
	
	rhs.setStateInitial(0);
	rhs.setStateFinal(1);

	EXPECT_TRUE(rhs.isValid());

	fa::Automaton fa = fa.createProduct(lhs,rhs);
	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(0,fa.countTransitions());
	EXPECT_EQ(1,fa.countStates());

}

TEST(createProduct,lhsLanguageEmpty){
	
	fa::Automaton lhs;
	static const std::vector<char> tab = {'p','q'};
	createAutomaton(lhs,3,tab);
	
	lhs.setStateFinal(2);

	EXPECT_TRUE(lhs.addTransition(0,'p',1));
	EXPECT_TRUE(lhs.addTransition(0,'p',2));
	EXPECT_TRUE(lhs.isValid());
	EXPECT_TRUE(lhs.isLanguageEmpty());

	fa::Automaton rhs;
	static const std::vector<char> tabe = {'p','q'};
	createAutomaton(rhs,3,tabe);

	EXPECT_TRUE(rhs.addTransition(0,'p',0));
	EXPECT_TRUE(rhs.addTransition(0,'p',1));
	EXPECT_TRUE(rhs.addTransition(1,'q',2));
	
	rhs.setStateInitial(0);
	rhs.setStateFinal(1);

	EXPECT_TRUE(rhs.isValid());
	

	fa::Automaton fa = fa.createProduct(lhs,rhs);

	EXPECT_TRUE(fa.isLanguageEmpty());
	EXPECT_EQ(1,fa.countStates());
	EXPECT_EQ(2,fa.countSymbols());
}

TEST(createProduct,rhsLanguageEmpty){
	
	fa::Automaton rhs;
	static const std::vector<char> tab = {'p','q'};
	createAutomaton(rhs,3,tab);
	
	rhs.setStateFinal(2);

	EXPECT_TRUE(rhs.addTransition(0,'p',1));
	EXPECT_TRUE(rhs.addTransition(0,'p',2));
	EXPECT_TRUE(rhs.isValid());
	EXPECT_TRUE(rhs.isLanguageEmpty());

	fa::Automaton lhs;
	static const std::vector<char> tabe = {'p','q'};
	createAutomaton(lhs,3,tabe);

	EXPECT_TRUE(lhs.addTransition(0,'p',0));
	EXPECT_TRUE(lhs.addTransition(0,'p',1));
	EXPECT_TRUE(lhs.addTransition(1,'q',2));
	
	lhs.setStateInitial(0);
	lhs.setStateFinal(1);

	EXPECT_TRUE(lhs.isValid());
	

	fa::Automaton fa = fa.createProduct(lhs,rhs);

	EXPECT_TRUE(fa.isLanguageEmpty());
	EXPECT_EQ(1,fa.countStates());
	EXPECT_EQ(2,fa.countSymbols());
}

TEST(createProduct,EmptyProductButEmptyWOrd){
	fa::Automaton lhs;
	static const std::vector<char> tab = {'p','q'};
	createAutomaton(lhs,3,tab);
	
	lhs.setStateInitial(0);
	lhs.setStateFinal(0);
	lhs.setStateFinal(2);

	EXPECT_TRUE(lhs.addTransition(0,'p',1));
	EXPECT_TRUE(lhs.addTransition(0,'p',2));
	EXPECT_TRUE(lhs.match(""));

	EXPECT_TRUE(lhs.isValid());
	
	fa::Automaton rhs;
	static const std::vector<char> tabe = {'h','y'};
	createAutomaton(rhs,3,tabe);

	EXPECT_TRUE(rhs.addTransition(0,'h',0));
	EXPECT_TRUE(rhs.addTransition(0,'h',1));
	EXPECT_TRUE(rhs.addTransition(1,'y',2));
	EXPECT_TRUE(rhs.match(""));
	
	rhs.setStateInitial(0);
	rhs.setStateFinal(0);
	rhs.setStateFinal(1);

	EXPECT_TRUE(rhs.isValid());

	fa::Automaton fa = fa.createProduct(lhs,rhs);
	EXPECT_TRUE(fa.isValid());
	EXPECT_EQ(0,fa.countTransitions());
	EXPECT_EQ(1,fa.countStates());
	EXPECT_FALSE(fa.isLanguageEmpty());
	EXPECT_TRUE(fa.match(""));
	EXPECT_FALSE(fa.match("pp"));
	EXPECT_FALSE(fa.match("h"));

}


TEST(createProduct,LoopOnExistingState){
	fa::Automaton lhs;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(lhs,3,tab);
	
	lhs.setStateInitial(0);
	lhs.setStateInitial(1);
	lhs.setStateFinal(2);
	
	EXPECT_TRUE(lhs.addTransition(0,'b',1));
	EXPECT_TRUE(lhs.addTransition(1,'a',2));
	EXPECT_TRUE(lhs.addTransition(2,'b',2));
	EXPECT_TRUE(lhs.isValid());


	fa::Automaton rhs;
	createAutomaton(rhs,4,tab);
	
	rhs.setStateInitial(0);
	rhs.setStateFinal(2);
	rhs.setStateFinal(3);
	
	EXPECT_TRUE(rhs.addTransition(0,'b',0));
	EXPECT_TRUE(rhs.addTransition(0,'a',1));
	EXPECT_TRUE(rhs.addTransition(1,'b',2));
	EXPECT_TRUE(rhs.addTransition(1,'a',3));
	EXPECT_TRUE(rhs.isValid());

	fa::Automaton fa = fa.createProduct(lhs,rhs);

	EXPECT_EQ(4,fa.countStates());
	EXPECT_EQ(3,fa.countTransitions());
	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.isLanguageEmpty());

	EXPECT_TRUE(fa.match("bab"));
	EXPECT_FALSE(fa.match("abbbbbbbbbbbbbbb"));

}

/*
 * hasEmptyIntersectionWith
 */


TEST(hasEmptyIntersectionWith,sameAutomate){
	fa::Automaton lhs;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(lhs,2,tab);
	
	lhs.setStateInitial(0);
	lhs.setStateFinal(1);
	
	EXPECT_TRUE(lhs.addTransition(0,'a',1));
	EXPECT_TRUE(lhs.addTransition(0,'b',1));
	EXPECT_TRUE(lhs.addTransition(1,'a',1));
	EXPECT_TRUE(lhs.isValid());

	fa::Automaton rhs = lhs;
	
	EXPECT_FALSE(lhs.hasEmptyIntersectionWith(rhs));
	EXPECT_FALSE(rhs.hasEmptyIntersectionWith(lhs));
}

TEST(hasEmptyIntersectionWith,himself){
	fa::Automaton lhs;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(lhs,2,tab);
	
	lhs.setStateInitial(0);
	lhs.setStateFinal(1);
	
	EXPECT_TRUE(lhs.addTransition(0,'a',1));
	EXPECT_TRUE(lhs.addTransition(0,'b',1));
	EXPECT_TRUE(lhs.addTransition(1,'a',1));
	EXPECT_TRUE(lhs.isValid());

	
	EXPECT_FALSE(lhs.hasEmptyIntersectionWith(lhs));
}

TEST(hasEmptyIntersectionWith,EmptyProduct){
	fa::Automaton lhs;
	static const std::vector<char> tab = {'p','q'};
	createAutomaton(lhs,3,tab);
	
	lhs.setStateInitial(0);
	lhs.setStateFinal(2);

	EXPECT_TRUE(lhs.addTransition(0,'p',1));
	EXPECT_TRUE(lhs.addTransition(0,'p',2));

	EXPECT_TRUE(lhs.isValid());
	
	fa::Automaton rhs;
	static const std::vector<char> tabe = {'h','y'};
	createAutomaton(rhs,3,tabe);

	EXPECT_TRUE(rhs.addTransition(0,'h',0));
	EXPECT_TRUE(rhs.addTransition(0,'h',1));
	EXPECT_TRUE(rhs.addTransition(1,'y',2));
	
	rhs.setStateInitial(0);
	rhs.setStateFinal(1);

	EXPECT_TRUE(rhs.isValid());

	EXPECT_FALSE(lhs.hasEmptyIntersectionWith(rhs));
}

TEST(hasEmptyIntersectionWith,lhsLanguageEmpty){
	fa::Automaton lhs;
	static const std::vector<char> tab = {'p','q'};
	createAutomaton(lhs,3,tab);
	
	lhs.setStateFinal(2);

	EXPECT_TRUE(lhs.addTransition(0,'p',1));
	EXPECT_TRUE(lhs.addTransition(0,'p',2));
	EXPECT_TRUE(lhs.isValid());
	EXPECT_TRUE(lhs.isLanguageEmpty());

	fa::Automaton rhs;
	static const std::vector<char> tabe = {'p','q'};
	createAutomaton(rhs,3,tabe);

	EXPECT_TRUE(rhs.addTransition(0,'p',0));
	EXPECT_TRUE(rhs.addTransition(0,'p',1));
	EXPECT_TRUE(rhs.addTransition(1,'q',2));
	
	rhs.setStateInitial(0);
	rhs.setStateFinal(1);

	EXPECT_TRUE(rhs.isValid());
	EXPECT_TRUE(lhs.hasEmptyIntersectionWith(rhs));
}

TEST(hasEmptyIntersectionWith,rhsLanguageEmpty){
	fa::Automaton rhs;
	static const std::vector<char> tab = {'p','q'};
	createAutomaton(rhs,3,tab);
	
	rhs.setStateFinal(2);

	EXPECT_TRUE(rhs.addTransition(0,'p',1));
	EXPECT_TRUE(rhs.addTransition(0,'p',2));
	EXPECT_TRUE(rhs.isValid());
	EXPECT_TRUE(rhs.isLanguageEmpty());

	fa::Automaton lhs;
	static const std::vector<char> tabe = {'p','q'};
	createAutomaton(lhs,3,tabe);

	EXPECT_TRUE(lhs.addTransition(0,'p',0));
	EXPECT_TRUE(lhs.addTransition(0,'p',1));
	EXPECT_TRUE(lhs.addTransition(1,'q',2));
	
	lhs.setStateInitial(0);
	lhs.setStateFinal(1);

	EXPECT_TRUE(lhs.isValid());
	EXPECT_TRUE(rhs.hasEmptyIntersectionWith(lhs));

}

TEST(hasEmptyIntersectionWith,LoopOnExistingState){
	fa::Automaton lhs;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(lhs,3,tab);
	
	lhs.setStateInitial(0);
	lhs.setStateInitial(1);
	lhs.setStateFinal(2);
	
	EXPECT_TRUE(lhs.addTransition(0,'b',1));
	EXPECT_TRUE(lhs.addTransition(1,'a',2));
	EXPECT_TRUE(lhs.addTransition(2,'b',2));
	EXPECT_TRUE(lhs.isValid());


	fa::Automaton rhs;
	createAutomaton(rhs,4,tab);
	
	rhs.setStateInitial(0);
	rhs.setStateFinal(2);
	rhs.setStateFinal(3);
	
	EXPECT_TRUE(rhs.addTransition(0,'b',0));
	EXPECT_TRUE(rhs.addTransition(0,'a',1));
	EXPECT_TRUE(rhs.addTransition(1,'b',2));
	EXPECT_TRUE(rhs.addTransition(1,'a',3));
	EXPECT_TRUE(rhs.isValid());

	EXPECT_FALSE(lhs.hasEmptyIntersectionWith(rhs));
	EXPECT_FALSE(rhs.hasEmptyIntersectionWith(lhs));
}

/*
 * readString
 */

TEST(readString,EmptyString){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.isValid());

	std::set<int> stateSet = fa.readString("");
	EXPECT_TRUE(stateSet.size() == 1);
	EXPECT_TRUE(stateSet.find(0) != stateSet.end());

}

TEST(readString,valid){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.isValid());

	std::set<int> stateSet = fa.readString("abab");
	EXPECT_TRUE(stateSet.size() == 1);
	EXPECT_TRUE(stateSet.find(2) != stateSet.end());

}

TEST(readString,validLeast2Result){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.isValid());

	std::set<int> stateSet = fa.readString("ab");
	EXPECT_TRUE(stateSet.size() == 2);
	EXPECT_TRUE(stateSet.find(0) != stateSet.end());
	EXPECT_TRUE(stateSet.find(2) != stateSet.end());

}

TEST(readString,noExistingSymbol){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.isValid());

	std::set<int> stateSet = fa.readString("c");
	EXPECT_TRUE(stateSet.size() == 0);

}


TEST(readString,noInitialState){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.isValid());

	std::set<int> stateSet = fa.readString("a");
	EXPECT_TRUE(stateSet.size() == 0);

}

TEST(readString,twoInitialStates){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateInitial(1);
	fa.setStateInitial(0);
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.isValid());

	std::set<int> stateSet = fa.readString("b");
	EXPECT_TRUE(stateSet.size() == 2);
	EXPECT_TRUE(stateSet.find(0) != stateSet.end());
	EXPECT_TRUE(stateSet.find(2) != stateSet.end());

}


/*
 * match
 */
TEST(match,EmptyString){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.isValid());
	
	EXPECT_FALSE(fa.match(""));

}

TEST(match,stringNotOnFinalState){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.isValid());
	
	EXPECT_FALSE(fa.match("a"));

}

TEST(match,validSingleCharString){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(1);
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.isValid());
	
	EXPECT_TRUE(fa.match("a"));

}

TEST(match,validLoopCharString){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(2);
	
	EXPECT_TRUE(fa.addTransition(0,'a',0));
	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.isValid());
	
	EXPECT_TRUE(fa.match("aaaaaaaaaab"));

}


TEST(match,symbolNotIn){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.isValid());
	
	EXPECT_FALSE(fa.match("c"));

}

TEST(match,twoInitialStates){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateInitial(0);
	fa.setStateInitial(1);
	fa.setStateFinal(2);
	
	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.isValid());
	
	EXPECT_TRUE(fa.match("b"));

}


TEST(match,noInitialState){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.isValid());
	
	EXPECT_FALSE(fa.match("b"));

}

TEST(match,noFinalState){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateInitial(0);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.isValid());
	
	EXPECT_FALSE(fa.match("b"));

}

TEST(match,EmptyWord){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,1,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(0);

	EXPECT_TRUE(fa.isValid());
	
	EXPECT_TRUE(fa.match(""));

}

/*
 * createDeterministic
 */
TEST(createDeterministic,twoInitialStates){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,5,tab);

	fa.setStateInitial(0);
	fa.setStateInitial(1);
	fa.setStateFinal(4);

	EXPECT_TRUE(fa.addTransition(0,'a',0));
	EXPECT_TRUE(fa.addTransition(0,'a',2));
	EXPECT_TRUE(fa.addTransition(0,'b',2));
	EXPECT_TRUE(fa.addTransition(2,'a',0));
	EXPECT_TRUE(fa.addTransition(2,'a',4));
	EXPECT_TRUE(fa.addTransition(1,'b',3));
	EXPECT_TRUE(fa.addTransition(1,'a',4));
	EXPECT_TRUE(fa.addTransition(1,'b',4));
	EXPECT_TRUE(fa.addTransition(3,'a',3));
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.isDeterministic());

	fa = fa.createDeterministic(fa);

	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isDeterministic());

}

TEST(createDeterministic,zeroInitialState){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,5,tab);

	fa.setStateFinal(4);

	EXPECT_TRUE(fa.addTransition(0,'a',0));
	EXPECT_TRUE(fa.addTransition(0,'a',2));
	EXPECT_TRUE(fa.addTransition(0,'b',2));
	EXPECT_TRUE(fa.addTransition(2,'a',0));
	EXPECT_TRUE(fa.addTransition(2,'a',4));
	EXPECT_TRUE(fa.addTransition(1,'b',3));
	EXPECT_TRUE(fa.addTransition(1,'a',4));
	EXPECT_TRUE(fa.addTransition(1,'b',4));
	EXPECT_TRUE(fa.addTransition(3,'a',3));
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.isDeterministic());

	fa = fa.createDeterministic(fa);

	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isDeterministic());

}

TEST(createDeterministic,alreadyDeterministic){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(0,'b',2));
	EXPECT_TRUE(fa.addTransition(1,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',2));

	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isDeterministic());
	fa = fa.createDeterministic(fa);
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isDeterministic());

}

/*
 * isIncludedIn
 */
TEST(isIncludedIn,sameAutomaton){
	fa::Automaton lhs;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(lhs,2,tab);
	
	lhs.setStateInitial(0);
	lhs.setStateFinal(1);
	
	EXPECT_TRUE(lhs.addTransition(0,'a',1));
	EXPECT_TRUE(lhs.addTransition(0,'b',1));
	EXPECT_TRUE(lhs.addTransition(1,'a',1));
	EXPECT_TRUE(lhs.isValid());

	fa::Automaton rhs = lhs;
	EXPECT_TRUE(lhs.isIncludedIn(rhs));
	EXPECT_TRUE(rhs.isIncludedIn(lhs));
	EXPECT_TRUE(lhs.isIncludedIn(lhs));

}

TEST(isIncludedIn,complement){
	fa::Automaton lhs;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(lhs,2,tab);
	
	lhs.setStateInitial(0);
	lhs.setStateFinal(1);
	
	EXPECT_TRUE(lhs.addTransition(0,'a',1));
	EXPECT_TRUE(lhs.addTransition(0,'b',1));
	EXPECT_TRUE(lhs.addTransition(1,'a',1));
	EXPECT_TRUE(lhs.isValid());
	
	fa::Automaton fa = fa.createComplement(lhs);
	
	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.isIncludedIn(lhs));
	EXPECT_FALSE(lhs.isIncludedIn(fa));
}

TEST(isIncludedIn,emptyLanguage){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.isValid());

	fa::Automaton faa;
	createAutomaton(faa,1,tab);

	EXPECT_TRUE(faa.isValid());
	EXPECT_TRUE(faa.isIncludedIn(fa));
}

TEST(isIncludedIn,loopA){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(0);
	fa.setStateFinal(2);
	
	EXPECT_TRUE(fa.addTransition(0,'a',0));
	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'b',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.isValid());

	fa::Automaton faa;
	createAutomaton(faa,1,{'a'});
	faa.setStateInitial(0);
	faa.setStateFinal(0);
	
	EXPECT_TRUE(faa.addTransition(0,'a',0));

	EXPECT_TRUE(faa.isValid());

	EXPECT_TRUE(faa.isIncludedIn(fa));

}

/*
 * createMinimalMoore
 */
TEST(createMinimalMoore,AlreadyMinimal){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',0));
	EXPECT_TRUE(fa.addTransition(0,'b',1));
	EXPECT_TRUE(fa.addTransition(1,'a',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.addTransition(2,'a',0));
	EXPECT_TRUE(fa.addTransition(2,'b',2));

	EXPECT_EQ(3,fa.countStates());

	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isDeterministic());
	EXPECT_TRUE(fa.isComplete());

	fa = fa.createMinimalMoore(fa);
	
	EXPECT_EQ(3,fa.countStates());
	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isDeterministic());
	EXPECT_TRUE(fa.isComplete());
}

TEST(createMinimalMoore,noInitialState){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',0));
	EXPECT_TRUE(fa.addTransition(0,'b',1));
	EXPECT_TRUE(fa.addTransition(1,'a',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.addTransition(2,'a',0));
	EXPECT_TRUE(fa.addTransition(2,'b',2));

	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.isDeterministic());
	EXPECT_TRUE(fa.isComplete());

	fa = fa.createMinimalMoore(fa);

	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isDeterministic());
	EXPECT_TRUE(fa.isComplete());
}


TEST(createMinimalMoore,td5ex17AlreadyCompletAndDeterministic){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,6,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(4);
	fa.setStateFinal(5);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'a',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.addTransition(2,'a',1));
	EXPECT_TRUE(fa.addTransition(0,'b',3));
	EXPECT_TRUE(fa.addTransition(3,'a',1));
	EXPECT_TRUE(fa.addTransition(3,'b',4));
	EXPECT_TRUE(fa.addTransition(4,'b',4));
	EXPECT_TRUE(fa.addTransition(4,'a',0));
	EXPECT_TRUE(fa.addTransition(2,'b',5));
	EXPECT_TRUE(fa.addTransition(5,'a',1));
	EXPECT_TRUE(fa.addTransition(5,'b',4));

	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isComplete());
	EXPECT_TRUE(fa.isDeterministic());
	EXPECT_EQ(6,fa.countStates());

	fa::Automaton faa = fa.createMinimalMoore(fa);

	EXPECT_TRUE(faa.isValid());
	EXPECT_TRUE(faa.isComplete());
	EXPECT_TRUE(faa.isDeterministic());
	EXPECT_EQ(3,faa.countStates());

	EXPECT_TRUE(fa.isIncludedIn(faa));
	EXPECT_TRUE(faa.isIncludedIn(fa));
}


/*
 * createMinimalBrzozowski
 */

TEST(createMinimalBrzozowski,AlreadyMinimal){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',0));
	EXPECT_TRUE(fa.addTransition(0,'b',1));
	EXPECT_TRUE(fa.addTransition(1,'a',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.addTransition(2,'a',0));
	EXPECT_TRUE(fa.addTransition(2,'b',2));

	EXPECT_EQ(3,fa.countStates());

	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isDeterministic());
	EXPECT_TRUE(fa.isComplete());

	fa = fa.createMinimalBrzozowski(fa);
	
	EXPECT_EQ(3,fa.countStates());
	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isDeterministic());
	EXPECT_TRUE(fa.isComplete());
}

TEST(createMinimalBrzozowski,noInitialState){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,3,tab);
	
	fa.setStateFinal(2);

	EXPECT_TRUE(fa.addTransition(0,'a',0));
	EXPECT_TRUE(fa.addTransition(0,'b',1));
	EXPECT_TRUE(fa.addTransition(1,'a',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.addTransition(2,'a',0));
	EXPECT_TRUE(fa.addTransition(2,'b',2));

	EXPECT_TRUE(fa.isValid());
	EXPECT_FALSE(fa.isDeterministic());
	EXPECT_TRUE(fa.isComplete());

	fa = fa.createMinimalBrzozowski(fa);

	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isDeterministic());
	EXPECT_TRUE(fa.isComplete());
}


TEST(createMinimalBrzozowski,td5ex17AlreadyCompletAndDeterministic){
	fa::Automaton fa;
	static const std::vector<char> tab = {'a','b'};
	createAutomaton(fa,6,tab);
	
	fa.setStateInitial(0);
	fa.setStateFinal(4);
	fa.setStateFinal(5);

	EXPECT_TRUE(fa.addTransition(0,'a',1));
	EXPECT_TRUE(fa.addTransition(1,'a',0));
	EXPECT_TRUE(fa.addTransition(1,'b',2));
	EXPECT_TRUE(fa.addTransition(2,'a',1));
	EXPECT_TRUE(fa.addTransition(0,'b',3));
	EXPECT_TRUE(fa.addTransition(3,'a',1));
	EXPECT_TRUE(fa.addTransition(3,'b',4));
	EXPECT_TRUE(fa.addTransition(4,'b',4));
	EXPECT_TRUE(fa.addTransition(4,'a',0));
	EXPECT_TRUE(fa.addTransition(2,'b',5));
	EXPECT_TRUE(fa.addTransition(5,'a',1));
	EXPECT_TRUE(fa.addTransition(5,'b',4));

	EXPECT_TRUE(fa.isValid());
	EXPECT_TRUE(fa.isComplete());
	EXPECT_TRUE(fa.isDeterministic());
	EXPECT_EQ(6,fa.countStates());

	fa::Automaton faa = fa.createMinimalBrzozowski(fa);

	EXPECT_TRUE(faa.isValid());
	EXPECT_TRUE(faa.isComplete());
	EXPECT_TRUE(faa.isDeterministic());
	EXPECT_EQ(3,faa.countStates());

	EXPECT_TRUE(fa.isIncludedIn(faa));
	EXPECT_TRUE(faa.isIncludedIn(fa));
}


/*
 * prettyPrint modelisation
 *//*
TEST(prettyPrint, Figure1Automatedexemple) {
  fa::Automaton fa;
  static const std::vector<char> tab = {'a','b'};
  createAutomaton(fa,5,tab);


  fa.setStateInitial(0);
  fa.setStateInitial(1);

  fa.setStateFinal(1);
  fa.setStateFinal(4);

  EXPECT_TRUE(fa.addTransition(0,'a',1));
  EXPECT_TRUE(fa.addTransition(0,'a',2));
  EXPECT_TRUE(fa.addTransition(0,'a',3));

  EXPECT_TRUE(fa.addTransition(1,'b',3));

  EXPECT_TRUE(fa.addTransition(2,'a',3));

  EXPECT_TRUE(fa.addTransition(3,'a',3));
  EXPECT_TRUE(fa.addTransition(3,'b',4));

  EXPECT_TRUE(fa.addTransition(4,'a',4));
  EXPECT_TRUE(fa.addTransition(4,'b',2));
  //fa.prettyPrint(std::cout);

  std::string const fichier("./img/figure1.dot");  //On ouvre le fichier
  std::ofstream monFlux(fichier.c_str());


  fa.dotPrint(monFlux);
}

TEST(prettyPrint, Figure2Automatedexemple) {
  fa::Automaton fa;
  static const std::vector<char> tab = {'a','b'};
  createAutomaton(fa,5,tab);

  fa.setStateInitial(0);
  fa.setStateInitial(1);

  fa.setStateFinal(1);
  fa.setStateFinal(4);

  EXPECT_TRUE(fa.addTransition(0,'a',1));
  EXPECT_TRUE(fa.addTransition(0,'a',2));
  EXPECT_TRUE(fa.addTransition(0,'a',3));
  EXPECT_TRUE(fa.addTransition(0,fa::Epsilon,4));

  EXPECT_TRUE(fa.addTransition(1,'b',3));

  EXPECT_TRUE(fa.addTransition(2,'a',3));

  EXPECT_TRUE(fa.addTransition(3,'a',3));
  EXPECT_TRUE(fa.addTransition(3,'b',4));

  EXPECT_TRUE(fa.addTransition(4,'a',4));
  EXPECT_TRUE(fa.addTransition(4,'b',2));
  //fa.prettyPrint(std::cout);

  const std::string file = "figure2";
  std::string const fichier("./img/"+file+".dot");  //On ouvre le fichier
  std::ofstream monFlux(fichier.c_str());

  //dot -Tpng figure2.dot -o figure2.png
  fa.dotPrint(monFlux);
}
*/

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

