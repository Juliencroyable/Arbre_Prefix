#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <fstream>
#include <algorithm>

using namespace std;

// Définition de constantes globales.
const int N = 4;
const char nucleotides[N] = {'A', 'C', 'G', 'T'};

// Définition d'une structure de données pour un arbre radix.
struct radix_tree_t {
    // Le préfixe de la chaîne associée à ce nœud.
    string prefix;
    // Les sous-arbres associés à ce nœud, sous la forme de paires (indice "int" et pointeur).
    vector<pair<int, radix_tree_t*>> next;
    // La position de la chaîne complète dans la liste de séquences.
    int sufix_pos;

    // Constructeur pour créer un nœud de l'arbre radix.
    radix_tree_t(string prefix="", int sufix_pos=-1) {
        // Initialisation des champs de la structure.
        this->prefix = prefix;
        this->sufix_pos = sufix_pos;
        for (int i = 0; i < N; i++) {
            // Ajout de N sous-arbres initialement vides.
            this->next.push_back(make_pair(i, (radix_tree_t*)nullptr));
        }
    }

    //Utilisation de l'opérateur destructeur "~" pour libérer la mémoire allouée aux sous-arbres.
    ~radix_tree_t() {
        for (int i = 0; i < N; i++) {
            if (next[i].second != nullptr) {
                delete next[i].second;
            }
        }
    }
};

int nucleotide_to_index(char nucleotide);
struct radix_tree_t;
void insert(radix_tree_t *root, const char *s, int pos);
void display_tree2D(ostream& os, radix_tree_t *root, int depth = 0);
void display_tree1D(ostream& os, radix_tree_t *root);

#endif // FONCTIONS_H
