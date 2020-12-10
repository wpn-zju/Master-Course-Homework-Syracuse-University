data Formula = Var Char
          | Not Formula
          | And Formula Formula
          | Or Formula Formula
          | Imply Formula Formula

f1 :: Formula
f1 = And (Var 'A') (Not (Var 'B'))

f2 :: Formula
f2 = Imply (And (Var 'A') (Var 'B')) (Var 'A')

f3 :: Formula
f3 = Imply (Var 'A') (And (Var 'A') (Var 'B'))

f4 :: Formula
f4 = Not (And (Not (And (Var 'A') (Or (Var 'A') (Var 'B')))) (Var 'B'))

implfree :: Formula -> Formula
implfree (Var b) = Var b
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

instance Show Formula where
  show (Var b) = show b
  show (Not p) = "!" ++ show p
  show (And p q) = "(" ++ show p ++ " & " ++ show q ++ ")"
  show (Or p q) = "(" ++ show p ++ " | " ++ show q ++ ")"
  show (Imply p q) = "(" ++ show p ++ " -> " ++ show q ++ ")"

--main = print (f4)
main = print (nnf f4)