#!/usr/bin/env python3

# string-blaster.py ---
#
# v. 0.1, 2023-01-19
# Frédéric Goualard, Nantes Université <Frederic.Goualard@univ-nantes.fr>
#
# Génération d'une liste de chaînes de caractères avec des recouvrements suffixe/préfixe
# à partir d'une chaîne générée aléatoirement pour tester les algorithmes de
# résolution du problème ``all pairs suffix-prefix''.
# On ajoute un recouvrement en préfixe de chaque sous-chaîne (sauf la première) et, avec
# une probabilité 0.5, en suffixe de chaque sous-chaîne sauf la dernière.
# Les sous-chaînes sont sauvés dans un ordre aléatoire dans le fichier résultat.
#
# Le programme accepte 5 paramètres:
# - le nom du fichier devant contenir le résultat (ou "-" pour envoyer sur stdout)
# - La longueur de la chaîne à générer aléatoirement
# - Le nombre de sous-chaînes à générer
# - la longueur minimum d'un recouvrement (optionnel)
# - la longueur maximum d'un recouvrement (optionnel)
#
# Si la longueur minimum de recouvrement n'est pas donnée, on prend `1` par défaut.
# Si la longueur maximum de recouvrement n'est pas donnée, on prend la longueur
# d'une séquence sans recouvrement ("longueur de la chaîne"/"nombre de sous-chaînes")
# moins 1 par défaut.
# Si l'on ne donne pas de longueur minimum de recouvrement, on ne doit pas donner non
# plus de longueur maximum.
# Exemple d'utilisation:
#   ./string-blaster.py out.dat 1000 10 1 40

import sys
import random
import textwrap

# Catching broken pipe error
# (https://stackoverflow.com/questions/11866792/how-to-prevent-errno-32-broken-pipe)
from signal import signal, SIGPIPE, SIG_DFL
signal(SIGPIPE,SIG_DFL)

# Seed for the random generator
SEED = 41
# Minimum size of a sub-string before adding an overlap
minsize = 1

def int_parameter(s):
    """
    Check that the string `s` can be cast into an integer. Return the
    corresponding integer if successful. Otherwise, abort the program
    with the error code 2.
    """
    try:
        return int(s)
    except ValueError:
        print(f"ERREUR: {s} n'est pas un entier",file=sys.stderr)
        exit(2)
        

def create_original_string(k):
    """
    Return a string of `k` bases chosen at random from ['A', 'T', 'C', 'G']
    """
    return ''.join(random.choices(['A','T','C','G'],k=k))


def prefix(s,k):
    """
    Return the `k` first letters of the string `s` 
    """
    return s[0:k]


def suffix(s,k):
    """
    Return the `k` last letters of the string `s`
    """
    return s[-(k+1):]


def blast_string(s, k, minov, maxov, f):
    """
    Split the string `s` into `k` parts and add overlaps of size greater or
    equal to `minov` and smaller or equal to `maxov`.
    """
    L = textwrap.wrap(s,len(s)//k)
    # We may have a k+1-th string if `k` does not divide len(s)
    # => we concatenate it to the penultimate string.
    if len(L) != k:
        L[-2] += L[-1]
        del L[-1]
    print(k,file=f)
    strands=[f"{L[0]}{prefix(L[1],random.randint(minov,maxov))}"]
    # Adding overlaps to the left of each substring except the very first.
    # With probability 0.5, we also add an overlap to the right for all substrings
    # but the last.
    for i in range(1,len(L)-1):
        if random.choice([True,False]):
            strands.append(f"{suffix(L[i-1],random.randint(minov,maxov))}{L[i]}{prefix(L[i+1],random.randint(minov,maxov))}")
        else:
            strands.append(f"{suffix(L[i-1],random.randint(minov,maxov))}{L[i]}")
    strands.append(f"{suffix(L[-2],random.randint(minov,maxov))}{L[-1]}")
    # The strands are saved in a random order in the file.
    random.shuffle(strands)
    print(*strands,sep="\n",file=f)
    print(s,file=f)
    
    
if __name__ == "__main__":
    if len(sys.argv) < 4:
        print(f"""
Use: \033[94m{sys.argv[0]} <filename> <strlen> <k> [<min_ov>] [<max_ov>]\033[0m

Create a random string with `strlen` bases from {{A,T,C,G}} and split 
it into `k` sub-strings {{L_1, \dots, L_k}}. Each substring `L_i` is 
then added an overlap with `L_{{i-1}}` and, with probability 0.5, 
an overlap with `L_{{i+1}}`. The size of the overlaps varies uniformly 
in `[min_ov, max_ov]`.
        
The program creates a file `filename` that contains:
- The number `k`;
- The list of substrings {{L_1, \dots, L_k}} separated by newlines;
- The original string.

The default value for `min_ov` is 1; the default value for `max_ov` 
is `strlen/k-1`.

If `filename` is "-", output the results to `stdout`; no file is created.
""",file=sys.stderr)
        exit(1)
    random.seed(SEED)
    filename = sys.argv[1]
    strsz = int_parameter(sys.argv[2])
    nsubs = int_parameter(sys.argv[3])
    if not (3 < nsubs < strsz//minsize):
        print(f"ERREUR: Le nombre de sous-chaînes doit être compris entre 3 et {strsz//minsize}.", file=sys.stderr)
        exit(3)

    # Is the minimum/maximum overlap is greater or equal to the size of the
    # smallest substring?
    sizesub = strsz // nsubs
    if len(sys.argv) < 5:
        minov = 1
        maxov = sizesub - 1
    else:
        minov = int_parameter(sys.argv[4])
        if len(sys.argv) < 6:
            maxov = sizesub - 1
        else:
            maxov = int_parameter(sys.argv[5])
            
    if sizesub <= minov or minov < 1 or sizesub <= maxov or maxov < minov:
        print(f"ERREUR: les overlaps doivent avoir une valeur dans [1,{sizesub-1}]",
              file=sys.stderr)
        exit(4)
        

    try:
        if filename == "-":
            blast_string(create_original_string(strsz), nsubs, minov, maxov, sys.stdout)
        else:
            with open(filename,"w") as f:
                blast_string(create_original_string(strsz), nsubs, minov, maxov, f)
    except PermissionError:
        print(f"ERREUR: Le fichier {filename} ne peut être ouvert en écriture",
              file=sys.stderr)
        exit(5)
    except Exception as e:
        print(f"ERREUR: {e}")
        exit(6)
            

    
