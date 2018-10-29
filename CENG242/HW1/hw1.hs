module Hw1 where

data Cell = H | P | O | X deriving (Read,Show,Eq)
data Result = Fail | Caught (Int,Int) deriving (Read,Show,Eq)
data Direction = N | S | E | W deriving (Read,Show,Eq)

--simulate :: [[Cell]] -> [(Direction, Direction)] -> Result--
-- DO NOT CHANGE ABOVE THIS LINE, WRITE YOUR CODE BELOW --

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
moveable area cord len dir = let
                                cord2 = move cord dir
                             in let
                                   x = fst(cord2)
                                   y = snd(cord2)
                                in
                                  if x >= fst(len) || y >= snd(len) || x < 0 || y < 0 then (False)
                                  else (area!!y!!x /= X)


find :: [[Cell]] -> (Int,Int) -> (Int,Int) -> Cell -> (Int, Int)
find area cord len persona = let
                                x = fst(cord)
                                y = snd(cord)
                             in 
                                if area!!y!!x /= persona then 
                                                             let
                                                                l1 = fst(len)
                                                                l2 = snd(len)
                                                             in if x + 1 < l1 then (find area (x + 1, y) len persona) else
                                                                                           if y + 1 < l2 then (find area (0, y + 1) len persona) else (-1, -1) 
                                else cord

pursue :: [[Cell]] -> (Int, Int) -> (Int, Int) -> (Int, Int) -> [(Direction, Direction)] ->(Int, Int)
pursue area len currp currh [] = if currp == currh then currp else (-1,-1)
pursue area len currp currh (x:dirs) = let
                                         hunter = fst(x) 
                                         prey = snd(x)
                                      in
                                        if currh == currp then currh
                                        else 
                                            let
                                               currh2 = if (moveable area currh len hunter) then (move currh hunter) else currh
                                               currp2 = if (moveable area currp len prey) then (move currp prey) else currp
                                            in 
                                              pursue area len currp2 currh2 (dirs)


simulate :: [[Cell]] -> [(Direction, Direction)] -> Result
simulate area dirs = let
                        l1 = len (head area)
                        l2 = len area 
                        currp = find area (0, 0) (l1, l2) P
                        currh = find area (0, 0) (l1, l2) H
                        result = pursue area (l1, l2) currp currh dirs
                     in
                       if (fst(result) /= -1) then Caught result else Fail