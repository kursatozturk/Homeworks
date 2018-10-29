module Hw2 where

import Data.List -- YOU MAY USE THIS MODULE FOR SORTING THE AGENTS

data Level = Newbie | Intermediate | Expert deriving (Enum, Eq, Ord, Show, Read)
data Hunter = Hunter {hID::Int, hlevel::Level, hEnergy::Int, hNumberOfCatches::Int, hActions::[Direction]} deriving (Eq, Show, Read)
data Prey = Prey {pID::Int, pEnergy::Int, pActions::[Direction]} deriving (Eq, Show, Read)
data Cell = O | X | H Hunter | P Prey | T deriving (Eq, Show, Read)
data Direction = N | S | E | W deriving (Eq, Show, Read)
type Coordinate = (Int, Int)
-- DO NOT CHANGE THE DEFINITIONS ABOVE. --


-- INSTANCES OF Ord FOR SORTING, UNCOMMENT AND COMPLETE THE IMPLEMENTATIONS --
instance Ord Hunter where
    compare (Hunter hID1 hlevel1 hEnergy1 hNumberOfCatches1 _) (Hunter hID2 hlevel2 hEnergy2 hNumberOfCatches2 _) =
        if hlevel1 == hlevel2 then
            if hEnergy1 == hEnergy2 then
                if hNumberOfCatches1 == hNumberOfCatches2 then
                    hID2 `compare` hID1
                else hNumberOfCatches1 `compare` hNumberOfCatches2
            else hEnergy1 `compare` hEnergy2
        else hlevel1 `compare` hlevel2 

instance Ord Prey where
    compare (Prey pId1 pEnergy1 _) (Prey pId2 pEnergy2 _) = 
        if pEnergy1 == pEnergy2 then
           pId2 `compare` pId1
        else (pEnergy1 `compare` pEnergy2)


-- WRITE THE REST OF YOUR CODE HERE --


len [] = 0
len (x:list) = 1 + len(list)

north :: (Int,Int) -> (Int,Int)
north cord = (fst(cord), snd(cord) - 1)

south :: (Int,Int) -> (Int,Int)
south cord = (fst(cord), snd(cord) + 1)

east :: (Int,Int) -> (Int,Int)
east cord = (fst(cord) + 1, snd(cord))

west :: (Int,Int) -> (Int,Int)
west cord = (fst(cord) - 1, snd(cord))

move :: (Int, Int) -> Direction -> (Int, Int)
move cord dir = case dir of N -> north cord
                            S -> south cord
                            E -> east cord
                            W -> west cord 

moveable :: [[Cell]] -> (Int, Int) -> (Int, Int) -> Direction -> Bool
moveable area cord (l1,l2) dir = let
                                (x,y) = move cord dir
                             in
                               if x >= l1 || y >= l2 || x < 0 || y < 0 then (False)
                               else ((area!!y!!x) /= X)

isitTrap::[[Cell]] -> Coordinate -> Bool
isitTrap area (x,y) =  (area!!y!!x) == T


isPreyorHunter::Cell -> [Char]
isPreyorHunter (H _) = "Hunter"
isPreyorHunter (P _) = "Prey"
isPreyorHunter x = "others"

isHunter::Cell -> Bool
isHunter a = (isPreyorHunter a) == "Hunter"

isPrey::Cell -> Bool
isPrey a = (isPreyorHunter a) == "Prey"

takeHunter::Cell -> Hunter
takeHunter (H a) = a

takePrey::Cell -> Prey
takePrey (P a) = a

hunterFinder::[[Cell]] -> Coordinate -> Coordinate -> [(Hunter, Coordinate)]
hunterFinder area (x,y) (l1,l2) = if (isHunter (area!!y!!x)) then
                                   if x + 1 < l1 then ((takeHunter (area!!y!!x)), (x,y)):(hunterFinder area (x + 1, y) (l1,l2)) 
                                   else 
                                    if y + 1 < l2 then ((takeHunter (area!!y!!x)), (x,y)):(hunterFinder area (0, y + 1) (l1,l2))
                                    else ((takeHunter (area!!y!!x)), (x,y)):[]
                                  else
                                   if x + 1 < l1 then (hunterFinder area (x + 1, y) (l1,l2)) 
                                   else
                                    if y + 1 < l2 then (hunterFinder area (0, y + 1) (l1,l2))
                                    else []

preyFinder::[[Cell]] -> Coordinate -> Coordinate -> [(Prey, Coordinate)]
preyFinder area (x,y) (l1,l2) = 
                         if (isPrey (area!!y!!x)) then
                           if x + 1 < l1 then ((takePrey (area!!y!!x)), (x,y)):(preyFinder area (x + 1, y) (l1,l2)) 
                           else 
                            if y + 1 < l2 then ((takePrey (area!!y!!x)), (x,y)):(preyFinder area (0, y + 1) (l1,l2))
                            else ((takePrey (area!!y!!x)), (x,y)):[]
                         else
                             if x + 1 < l1 then (preyFinder area (x + 1, y) (l1,l2)) 
                             else
                              if y + 1 < l2 then (preyFinder area (0, y + 1) (l1,l2))
                              else []

preysAndHunters::[[Cell]] -> ([(Hunter, Coordinate)],[(Prey, Coordinate)])
preysAndHunters area = let
                          l1 = len (head area)
                          l2 = len area
                          hunters = hunterFinder area (0,0) (l1,l2)
                          preys = preyFinder area (0,0) (l1,l2)
                       in (hunters,preys)

makeHuntersmove::[[Cell]] -> [(Hunter, Coordinate)] -> Coordinate -> [(Hunter, Coordinate)]
makeHuntersmove _ [] _ = []
makeHuntersmove area (first:hunters) len = let
                                         (x:actions) = hActions (fst first)
                                      in 
                                        if (hEnergy (fst first)) > 9 then
                                          let  
                                             cord = if moveable area (snd first) len x then move (snd first) x else (snd first)
                                          in let
                                                newH = Hunter (hID (fst first)) (hlevel (fst first)) ((hEnergy (fst first)) - 1) (hNumberOfCatches (fst first)) actions
                                             in (newH,cord):(makeHuntersmove area hunters len)
                                        else let
                                                newH = Hunter (hID (fst first)) (hlevel (fst first)) ((hEnergy (fst first)) + 1) (hNumberOfCatches (fst first)) actions
                                                cord = snd first
                                            in (newH,cord):(makeHuntersmove area hunters len)

makePreysmove::[[Cell]] -> [(Prey, Coordinate)] -> Coordinate -> [(Prey, Coordinate)]
makePreysmove _ [] _ = []
makePreysmove area (first:preys) len = let
                                         (x:actions) = pActions (fst first)
                                       in 
                                         if (pEnergy (fst first)) > 9 then
                                          let
                                             cord = if moveable area (snd first) len x then (move (snd first) x) else (snd first)
                                             energy = if isitTrap area cord then 
                                                        if ((pEnergy (fst first)) - 11) < 0 then 0 else ((pEnergy (fst first)) - 11)
                                                      else ((pEnergy (fst first)) - 1)

                                          in let
                                                newP = Prey (pID (fst first)) energy actions
                                             in (newP,cord):(makePreysmove area preys len)
                                         else let
                                                 cord = snd first
                                                 energy = if isitTrap area cord then 0 else (pEnergy (fst first)) + 1                                                 
                                              in let
                                                    newP = Prey (pID (fst first)) energy actions
                                                 in (newP,cord):(makePreysmove area preys len)

catchprey::(Hunter, Coordinate) -> [(Prey, Coordinate)] -> Int -> ([(Prey, Coordinate)],Hunter)
catchprey hunter [] _ = ([],(fst hunter)) 
catchprey hunter (prey:preys) catched = let 
                                           cord1 = snd hunter
                                           cord2 = snd prey
                                        in if cord1 == cord2 then
                                            let
                                               newenergy = if ((hEnergy (fst hunter)) + 20) > 100 then 100 else ((hEnergy (fst hunter)) + 20)
                                               newcatched = catched + 1 
                                               newH = Hunter (hID (fst hunter)) (hlevel (fst hunter)) newenergy ((hNumberOfCatches (fst hunter)) + 1) (hActions (fst hunter))
                                            in if newcatched == 2 then (preys,newH)
                                               else catchprey (newH,(snd hunter)) preys newcatched                                              
                                           else let
                                                   result = catchprey hunter preys catched
                                                in ((prey:(fst result)),(snd result))

catcher::[(Hunter, Coordinate)]->[(Prey, Coordinate)]->([(Hunter, Coordinate)],[(Prey, Coordinate)])
catcher [] [] = ([],[])
catcher [] preys = ([],preys)
catcher hunters [] = (hunters,[])
catcher (hunter:hunters) preys = let
                                    (preylist,newH) = catchprey hunter preys 0
                                 in let
                                       result = catcher hunters preylist
                                       newhunter = (newH,(snd hunter))
                                    in ((newhunter:(fst result)), (snd result))
                                      

pursue::[[Cell]] -> [(Hunter, Coordinate)] -> [(Prey, Coordinate)] -> Int -> Coordinate -> ([(Hunter, Coordinate)], [(Prey, Coordinate)])
pursue _ hunters preys 0 _ = (hunters, preys)
pursue _ hunters [] _ _ =(hunters, []) 
pursue area hunters preys numberofactions len = let
                                                   hunterlist = makeHuntersmove area hunters len
                                                   preylist = makePreysmove area preys len
                                                in let
                                                      (movedh,movedp) = catcher (reverse (sort hunterlist))  (sort preylist)
                                                   in pursue area movedh movedp (numberofactions-1) len


simulate::[[Cell]] -> ([(Hunter, Coordinate)], [(Prey, Coordinate)])
simulate area = let
                   (hunterss,preyss) = preysAndHunters area
                   l1 = len (head area)
                   l2 = len area
                in let
                      hunters = reverse (sort hunterss)
                      preys =  sort preyss
                      numberofactions = len (hActions (fst(hunterss!!0)))
                      (hs,ps) = (pursue area hunters preys numberofactions (l1,l2))
                   in ((reverse (sort hs)),(sort ps))
