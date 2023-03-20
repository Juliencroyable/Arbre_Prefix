#include "fonctions.h"

//Fonction convertissant le nucléotide en son indice correspondant (0 à 3).
int nucleotide_to_index(char nucleotide) {
    for (int i = 0; i < N; i++) {
        if (nucleotides[i] == nucleotide) {
            return i;
        }
    }
    return -1;
}

//Fonction insérant une chaîne de caractères dans l'arbre radix.
void insert(radix_tree_t *root, const char *s, int pos) {
    radix_tree_t *current_node = root;
    int i = 0;
    while (s[i] != '\0') {
        int index = nucleotide_to_index(s[i]);
        if (current_node->next[index].second == nullptr) {
            //Si le sous-arbre correspondant au nucléotide courant n'existe pas, on le créer.
            current_node->next[index].second = new radix_tree_t(string(s + i), pos);
            return;
        } else {
            //Sinon, on trouve le sous-arbre correspondant.
            radix_tree_t *child_node = current_node->next[index].second;
            int j = 0;
            //On trouve la longueur de la chaîne commune entre le préfixe existant et la nouvelle chaîne.
            while (s[i + j] != '\0' && child_node->prefix[j] == s[i + j]) {
                j++;
            }
            //Si la chaîne courante est identique au préfixe du sous-arbre, on continue la recherche.
            if (j == child_node->prefix.size()) {
                i += j;
                current_node = child_node;
            //Sinon, on doit diviser le sous-arbre existant en deux parties, la 1ère contenant la chaine commune jusqu'à la position "j", et la deuxième contenant le reste.
            } else {
                //On sauvegarde le préfixe et l'indice de la chaîne existante dans des variables "old_prefix" et "old_pos".
                string old_prefix = child_node->prefix;
                int old_pos = child_node->sufix_pos;
                //On modifie le préfixe et l'indice du nœud existant pour ne conserver que la sous-chaîne commune jusqu'à la position j de la chaîne existante, et on indique qu'il ne s'agit plus d'une feuille.
                child_node->prefix = child_node->prefix.substr(0, j);
                child_node->sufix_pos = -1;
                //On crée un nouveau nœud pour stocker la sous-chaîne restante de la chaîne existante, à partir de la position j.
                radix_tree_t *new_child_node = new radix_tree_t(old_prefix.substr(j), old_pos);
                //On récupère l'indice correspondant à la lettre suivant la sous-chaîne commune dans la chaîne en cours d'insertion, pour pouvoir insérer le nouveau nœud à la bonne position.
                int new_index = nucleotide_to_index(old_prefix[j]);
                //On copie les pointeurs pour les enfants du nœud existant dans le nouveau nœud, puis on réinitialise ces pointeurs dans le nœud existant.
                new_child_node->next = child_node->next;
                child_node->next = vector<pair<int, radix_tree_t*>>(N, make_pair(-1, (radix_tree_t*)nullptr));
                //On insère le nouveau nœud entre les deux parties en modifiant le pointeur pour l'enfant correspondant dans le nœud existant.
                child_node->next[new_index].second = new_child_node;
                //On finit par mettre à jour le noeud courant, il passe en noeud existant.
                current_node = child_node;
                i += j;
                
            }
        }
    }
}

//Fonction affichant l'arbre radix de manière 2D avec indentation selon la profondeur.
void display_tree2D(ostream& os, radix_tree_t *root, int depth) {
    if (root == nullptr) {
        return;
    }
    //On affiche le préfixe du nœud avec un astérisque si le nœud correspond à la fin d'une séquence.
    os << string(depth, ' ') << "[" << root->prefix << "]" << (root->sufix_pos != -1 ? "*" : "") << endl;
    for (int i = 0; i < N; i++) {
        //On appelle récursivement la fonction pour afficher les sous-arbres.
        display_tree2D(os, root->next[i].second, depth + 1);
    }
}

//Fonction affichant l'arbre radix en 1D, sans indentation ni profondeur.
void display_tree1D(ostream& os, radix_tree_t *root) {
    if (root == nullptr) {
        return;
    }
    os << "[" << root->prefix << "]";
    if (root->sufix_pos != -1) {
        os << "*";
    }
    for (auto const& child : root->next) {
        display_tree1D(os, child.second);
    }
}

