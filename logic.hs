{-# LANGUAGE PatternSynonyms #-}
{-# OPTIONS -Wall #-}

import Data.Set (Set)
import qualified Data.Set as Set
import Data.Map.Strict (Map, (!))
import qualified Data.Map.Strict as Map


data Statement = Var Char
               | Not Statement
               | AndL [Statement]
               | OrL [Statement]
            --   | And Statement Statement
            --   | Or  Statement Statement
            -- AndL/OrL was originally defined like the pattern below.
            -- They were redefined like this do to their associative properties.
            -- I'm not convinced this is the best choice...
            -- In particular, the malformed statement ``AndL [p]" is now possible.
               | (:=>) Statement Statement
               | (:<=>) Statement Statement -- This could (should) also be [Statement]
               deriving (Eq, Ord)
            -- It cannot be stressed enough how shallow this form of equality is.
            -- `Ord' is required for sets. I shudder to imagine how order is being
            --   defined upon a tree structure.

pattern And :: Statement -> Statement -> Statement
pattern And p q = AndL [p,q]
pattern Or :: Statement -> Statement -> Statement
pattern Or p q = OrL [p,q]

isAnd :: Statement -> Bool
isAnd (AndL _) = True
isAnd _ = False

isOr :: Statement -> Bool
isOr (OrL _) = True
isOr _ = False

isFlat :: Statement -> Bool
isFlat (Var _) = True
isFlat (Not p) = isFlat p
isFlat (AndL ps) = not (any isAnd ps) &&  all isFlat ps
isFlat (OrL ps) = not (any isOr ps) &&  all isFlat ps
isFlat (p :=> q) = isFlat p && isFlat q
isFlat (p :<=> q) = isFlat p && isFlat q

flattenStatement :: Statement -> Statement
flattenStatement ((p `And` q) `And` (r `And` s)) = AndL [p,q,r,s]
flattenStatement ((p `And` q) `And` r) = AndL [p,q,r]
flattenStatement (p `And` (q `And` r)) = AndL [p,q,r]
flattenStatement ((p `Or` q) `Or` (r `Or` s)) = OrL [p,q,r,s]
flattenStatement ((p `Or` q) `Or` r) = OrL [p,q,r]
flattenStatement (p `Or` (q `Or` r)) = OrL [p,q,r]

parenShow :: Statement -> String
parenShow (Var p) = [p]
parenShow (Not (Var p)) = '~':[p]
parenShow (Not p) = "~(" ++ parenShow p ++ ")"
parenShow ((Var p) `And` (Var q)) =  [p] ++ " ∧ " ++ [q]
parenShow (p@(_ `And` _) `And` (Var q)) =  parenShow p ++ " ∧ " ++ [q]
parenShow (p@(_ `And` _) `And` (Var q)) =  parenShow p ++ " ∧ " ++ [q]
parenShow (p@(_ `And` _) `And` q@(Not _)) =  parenShow p ++ " ∧ " ++ parenShow q
parenShow (p@(_ `And` _) `And` q@(_ `And` _)) =  parenShow p ++ " ∧ " ++ parenShow q
parenShow (p@(_ `And` _) `And` q@(_ `And` _)) =  parenShow p ++ " ∧ " ++ parenShow q
parenShow (p@(_ `And` _) `And` q@(_ `And` _)) =  parenShow p ++ " ∧ " ++ parenShow q
parenShow (p@(_ `Or` _) `Or` q@(_ `Or` _)) =  parenShow p ++ " ∨ " ++ parenShow q
parenShow (p :=> q) = parenShow p ++ " => " ++ parenShow q
parenShow (p :<=> q) = parenShow p ++ " <=> " ++ parenShow q

instance Show Statement where
    show (Var p) = [p]
    show (Not p) = "~(" ++ show p ++ ")"
    show (p@(_ `And` _) `And` q) =  show p ++ " ∧ (" ++ show q ++ ")"
    show (And p q) = "(" ++ show p ++ ") ∧ (" ++ show q ++ ")"
    show (p@(_ `Or` _) `Or` q) =  show p ++ " ∨ (" ++ show q ++ ")"
    show (Or p q) = "(" ++ show p ++ ") ∨ (" ++ show q ++ ")"
    show (p :=> q) = "(" ++ show p ++ ") => (" ++ show q ++ ")"
    show (p :<=> q) = "(" ++ show p ++ ") <=> (" ++ show q ++ ")"

toTex :: Statement -> String
toTex (Var p) = [p]
toTex (Not p) = "\\sim (" ++ toTex p ++ ")"
toTex (And p q) = "(" ++ toTex p ++ ") \\land (" ++ toTex q ++ ")"
toTex (Or p q) = "(" ++ toTex p ++ ") \\lor (" ++ toTex q ++ ")"
toTex (p :=> q) = "(" ++ toTex p ++ ") \\Rightarrow (" ++ toTex q ++ ")"
toTex (p :<=> q) = "(" ++ toTex p ++ ") \\Leftrightarrow (" ++ toTex q ++ ")"

collectVars :: Statement -> Set Char
collectVars (Var p) = Set.singleton p
collectVars (Not statement) = collectVars statement
collectVars (AndL statements) = foldr Set.union Set.empty (fmap collectVars statements)
collectVars (OrL statements) = foldr Set.union Set.empty (fmap collectVars statements)
collectVars (p :=> q) = Set.union (collectVars p) (collectVars q)
collectVars (p :<=> q) = Set.union (collectVars p) (collectVars q)

collectExpressions :: Statement -> Set Statement
collectExpressions v@(Var _) = Set.singleton v
collectExpressions n@(Not p) =
    Set.union (Set.singleton n) (collectExpressions p)
collectExpressions a@(AndL ps) =
    Set.union (Set.singleton a) (Set.unions $ fmap collectExpressions ps)
collectExpressions o@(OrL ps) =
    Set.union (Set.singleton o) (Set.unions $ fmap collectExpressions ps)
collectExpressions i@(p :=> q) =
    Set.unions [Set.singleton i, collectExpressions p, collectExpressions q]
collectExpressions e@(p :<=> q) =
    Set.unions [Set.singleton e, collectExpressions p, collectExpressions q]

evaluateStatementUnsafe :: Map Char Bool -> Statement -> Bool
evaluateStatementUnsafe values (Var p) = values ! p
evaluateStatementUnsafe values (Not p) = not (evaluateStatementUnsafe values p)
evaluateStatementUnsafe values (AndL ps) = all (evaluateStatementUnsafe values) ps
evaluateStatementUnsafe values (OrL ps) = any (evaluateStatementUnsafe values) ps
evaluateStatementUnsafe values (p :=> q) =
    not (evaluateStatementUnsafe values p) || evaluateStatementUnsafe values q
evaluateStatementUnsafe values (p :<=> q) =
    evaluateStatementUnsafe values p == evaluateStatementUnsafe values q

cancelNots :: Statement -> Statement
cancelNots (Var p) = Var p
cancelNots (Not (Not p)) = cancelNots p
cancelNots (p `And` q) = (cancelNots p) `And` (cancelNots q)
cancelNots (p `Or` q) = (cancelNots p) `Or` (cancelNots q)
cancelNots (p :=> q) = (cancelNots p) :=> (cancelNots q)
cancelNots (p :<=> q) = (cancelNots p) :<=> (cancelNots q)
cancelNots (Not p) = Not (cancelNots p)

--p = Var 'p'
--q = Var 'q'

deMorgans :: Statement -> Statement
--deMorgans (Not (AndL ps)) = OrL (fmap (Not . deMorgans) ps)
--deMorgans (Not (OrL ps)) = AndL (fmap (Not . deMorgans) ps)
deMorgans (Not (And p q)) = (Not (deMorgans p)) `Or` (Not (deMorgans q))
deMorgans (Not (Or p q)) = (Not (deMorgans p)) `And` (Not (deMorgans q))
deMorgans (Var p) = Var p
deMorgans (Not p) = Not (deMorgans p)
deMorgans (And p q) = And (deMorgans p) (deMorgans q)
deMorgans (Or p q) = Or (deMorgans p) (deMorgans q)
--deMorgans (AndL ps) = AndL (fmap deMorgans ps)
--deMorgans (OrL ps) = OrL (fmap deMorgans ps)
deMorgans (p :=> q) = (deMorgans p) :=> (deMorgans q)
deMorgans (p :<=> q) = (deMorgans p) :<=> (deMorgans q)

removeImplication :: Statement -> Statement
removeImplication (p :=> q) = (Not (removeImplication p)) `Or` (removeImplication q)
removeImplication (Var p) = Var p
removeImplication (Not p) = Not (removeImplication p)
removeImplication (p `And` q) = (removeImplication p) `And` (removeImplication q)
removeImplication (p `Or` q) = (removeImplication p) `Or` (removeImplication q)
removeImplication (p :<=> q) = (removeImplication p) :<=> (removeImplication q)

removeEquivalence :: Statement -> Statement
removeEquivalence (p :<=> q) = ((Not (removeEquivalence p)) `Or` (removeEquivalence q)) `And` ((Not (removeEquivalence q)) `Or` (removeEquivalence p))
removeEquivalence (Var p) = Var p
removeEquivalence (Not p) = Not (removeEquivalence p)
removeEquivalence (p `And` q) = (removeEquivalence p) `And` (removeEquivalence q)
removeEquivalence (p `Or` q) = (removeEquivalence p) `Or` (removeEquivalence q)
removeEquivalence (p :=> q) = (removeEquivalence p) :=> (removeEquivalence q)

removeOr :: Statement -> Statement
removeOr (p `Or` q) = Not ((Not (removeOr p)) `And` (Not (removeOr q)))
removeOr (Var p) = Var p
removeOr (Not p) = Not (removeOr p)
removeOr (p `And` q) = (removeOr p) `And` (removeOr q)
removeOr (p :=> q) = (removeOr p) :=> (removeOr q)
removeOr (p :<=> q) = (removeOr p) :<=> (removeOr q)
