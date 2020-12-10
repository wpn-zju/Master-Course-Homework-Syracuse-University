data Formula = Const Bool
          | Var Char
          | Not Formula
          | And Formula Formula
          | Or Formula Formula
          | Imply Formula Formula

pa :: Formula
pa = Var 'A'

pb :: Formula
pb = Var 'B'

p1 :: Formula
p1 = And (Var 'A') (Not (Var 'A'))

p2 :: Formula
p2 = Imply (And (Var 'A') (Var 'B')) (Var 'A')

p3 :: Formula
p3 = Imply (Var 'A') (And (Var 'A') (Var 'B'))

p4 :: Formula
p4 = Imply (And (Var 'A') (Imply (Var 'A') (Var 'B'))) (Var 'B')

implfree :: Formula -> Formula
implfree (Const b) = Const b
implfree (Var x) = Var x
implfree (Not p) = Not (implfree p)
implfree (And p q) = And (implfree p) (implfree q)
implfree (Or p q) = Or (implfree p) (implfree q)
implfree (Imply p q) = And (Not (implfree p)) (implfree q)

nnf :: Formula -> Formula -- Implication Free
nnf (Imply p q) = nnf (implfree (Imply p q))
nnf (Not (Not p)) = nnf p
nnf (Not (And p q)) = Or (nnf (Not p)) (nnf (Not q))
nnf (Not (Or p q)) = And (nnf (Not p)) (nnf (Not q))
nnf (Not p) = Not (nnf p)
nnf (And p q) = And (nnf p) (nnf q)
nnf (Or p q) = Or (nnf p) (nnf q)
nnf p = p

-- instance Show Formula where
--   show (Const b) = show b
--   show (Var x) = show x
--   show (Not p) = "!" ++ show p
--   show (And p q) = "(" ++ show p ++ " & " ++ show q ++ ")"
--   show (Or p q) = "(" ++ show p ++ " | " ++ show q ++ ")"
--   show (Imply p q) = "(" ++ show p ++ " -> " ++ show q ++ ")"

instance Show Formula where
  show (Const b) = show b
  show (Var x) = show x
  show (Not p) = "~" ++ "(" ++ show p ++ ")"
  show (And p q) = "(" ++ show p ++ ")" ++ "^" ++ "(" ++ show q ++ ")"
  show (Or p q) = "(" ++ show p ++ ")" ++ "|" ++ "(" ++ show q ++ ")"
  show (Imply p q) = "(" ++ show p ++ ")" ++ "->" ++ "(" ++ show q ++ ")"

--main = print p4

data RTree a = Vertex a [RTree a]

preorder :: RTree Int -> [Int]
preorder (Vertex rt cl) = foldl (++) [rt] [preorder c | c <- cl]

main = print (preorder (Vertex 1 [(Vertex 4 [Vertex 2 [(Vertex 5 []), (Vertex 6 [])]]), (Vertex 3 [])]))