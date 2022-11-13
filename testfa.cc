
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




void testDotPrint(const fa::Automaton& mamiRobot,std::string file){
  std::string const fichier("./img/"+file+".dot");  //On ouvre le fichier
  std::ofstream monFlux(fichier.c_str());

  //dot -Tpng figure2.dot -o figure2.png
  mamiRobot.dotPrint(monFlux);
}

TEST(createComplete, test) {
  fa::Automaton fa;
  createAutomaton(fa,4,{'a','b'});
  fa.setStateInitial(0);
  fa.addTransition(0,'a',1);
  fa.addTransition(1,'a',0);
  testDotPrint(fa,"test");
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

  testDotPrint(fa,"figure4");

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

  /*std::string motReconnus = "aaabb";
  EXPECT_TRUE(fa.match(motReconnus));
  */
  fa = fa.createComplement(fa);
  
  EXPECT_TRUE(fa.isValid());
  EXPECT_TRUE(fa.isComplete());
  EXPECT_TRUE(fa.isDeterministic());

  EXPECT_EQ(2u,fa.countSymbols());
  EXPECT_TRUE(fa.hasSymbol('a'));
  EXPECT_TRUE(fa.hasSymbol('b'));
  EXPECT_TRUE(fa.isStateFinal(0));
  EXPECT_TRUE(fa.isStateFinal(2));
  EXPECT_FALSE(fa.isStateFinal(1));
  EXPECT_TRUE(fa.isStateInitial(0));
  //EXPECT_FALSE(fa.match(motReconnus));
  //EXPECT_TRUE(fa.match("bbbba"));

  //testDotPrint(fa,"complement");

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
  //EXPECT_FALSE(fa.match("bab"));
  //EXPECT_TRUE(fa.match("aba"));
  testDotPrint(fa,"mirror1");

  fa = fa.createMirror(fa);
	EXPECT_TRUE(fa.isValid());
  //EXPECT_TRUE(fa.match("bab"));
  //EXPECT_FALSE(fa.match("aba"));

  testDotPrint(fa,"mirror2");
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




/*
 * prettyPrint modelisation
 */
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


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

