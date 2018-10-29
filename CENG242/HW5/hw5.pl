:- module(hw5, [configuration/3]).
:- [hardware].
/*
findOccurences(El,[],[]).
findOccurences(El,[Head|List],[A|Res]) :- (member(makeTrue(El,B),[Head|List]),not(member(makeTrue(El,B),List))), !, findOccurences(El,List,Res), A = B.
findOccurences(El,[Head|List],Res) :- findOccurences(El,List,Res).
*/
isAdjacent(Section1, Section2) :- adjacent(Section1, Section2) ; adjacent(Section2, Section1).

emptyIt([]).

helperOuter(_,[],0).
helperOuter(Section, [Head|List], Res) :- isAdjacent(Section, Head), !,
  helperOuter(Section, List, Res2), Res is Res2 + 1.
helperOuter(Section, [Head|List], Res) :- helperOuter(Section, List, Res).

hasOuter(Section) :- sections(List), helperOuter(Section, List, NumberofAdj), NumberofAdj =\= 4.


getOuterEdgeList(El, Constraints, [A|R]) :-
  member(outer_edge(El), Constraints), !,
  A = outer_edge(El),
  emptyIt(R).
getOuterEdgeList(El, Constraints, R) :- emptyIt(R).


getCloseTo(_, [], []).
getCloseTo(El, Constraints, [A|R]) :-
  (member(close_to(El, B), Constraints);member(close_to(B, El), Constraints)), !,
   A = B,
   emptyIt(R).
getCloseTo(El, Constraints, R) :- emptyIt(R).

placeIt(Comp, [], [], SectionList, Placement, [put(Comp, Sect)|Placement]) :-
  member(Sect, SectionList),
  not(member(put(_, Sect), Placement)).

placeIt(Comp,[OuterList], [], SectionList, Placement, [put(Comp, Sect)|Placement]) :-
  member(Sect, SectionList),
  hasOuter(Sect),
  not(member(put(_, Sect), Placement)).

placeIt(Comp, [], [CloseComp], SectionList, Placement, [put(Comp, AdjCand)|Placement]) :-
  member(put(CloseComp, B), Placement), !,
  isAdjacent(AdjCand, B),
  not(member(put(_, AdjCand), Placement)).

placeIt(Comp, [], [CloseComp], SectionList, Placement, [put(Comp, Sect)|Placement]) :-
  member(Sect, SectionList),
  not(member(put(_, Sect), Placement)).

confHelper([],_,Placement,Placement).
confHelper([Comp|Components], Constraints, Container, Placements) :-
  getOuterEdgeList(Comp, Constraints, OuterList),
  getCloseTo(Comp, Constraints, CloseList),
  sections(SectionList),
  confHelper(Components, Constraints, Container, PlacementHelp),
  placeIt(Comp, OuterList, CloseList, SectionList, PlacementHelp, Placements).


configuration(Components, Constraints, Placements) :-
  confHelper(Components, Constraints, [], Placements).



/*

You are really ugly did u know that? */
