#include "fonctions.h"

int main() {
    ifstream fichier_entree("out.dat"); //Le programme tente d'ouvrir le fichier "out.dat".

    if (!fichier_entree) {
        cerr << "Impossible d'ouvrir le fichier" << endl;
        return 1;
    }

    vector<string> sequences; //Vecteur permettant de stocker les séquences (vecteur et non tableau car la taille de "out.dat" varie en fonction des paramètres de l'utilisateur).
    string ligne;
    getline(fichier_entree, ligne);
    while (getline(fichier_entree, ligne)) {
        sequences.push_back(ligne);
    }

    fichier_entree.close(); 

    //Création d'un arbre radix vide initial.
    radix_tree_t* root = new radix_tree_t();
    //Insérer chaque séquence du fichier dans l'arbre.
    for (int i = 0; i < sequences.size(); ++i) {
        insert(root, sequences[i].c_str(), i);
    }

    ofstream outfile("arbre_prefix.txt"); //Création d'un fichier de sortie.
    if (!outfile) {
        cerr << "Impossible d'ouvrir le fichier de sortie" << endl;
        return 1;
    }

    //Implentation de l'arbre radix 2D dans le fichier.
    outfile << "Displaying 2D tree:" << endl;
    display_tree2D(outfile, root);

    //Implentation de l'arbre radix 1D dans le fichier.
    outfile << "Displaying 1D tree:" << endl;
    display_tree1D(outfile, root);

    outfile.close();

    return 0;
}