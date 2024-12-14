#include <iostream>
#include <fstream>
#include <vector>
#include "nlohmann/json.hpp"
#include <iomanip>
#include <chrono>
#include <ctime>
using namespace std;
using json = nlohmann::json;

  struct Depence{
        int id;
        string date;
        string description;
        float montant;

    };

void afficherIntroduction() {

    cout << "\n\nVoici les commandes disponibles :\n";
    cout << "1. Ajouter une depense\n";
    cout << "2. Lister les depenses\n";
    cout << "3. Supprimer une depense\n";
    cout << "4. Quitter\n";
    cout << "==============================================\n";
}
void enregistrerDepensesDansFichier(vector <Depence>& depenses){
    json data;
    for(const auto dep :depenses){
        json eachobj;
        eachobj["id"] = dep.id;
         eachobj["date"] = dep.date;
        eachobj["description"] = dep.description;
        eachobj["montant"] =  dep.montant;
        data.push_back(eachobj);
    }
    //ecrire dans le fichier
    ofstream fichier("depenses.json");
    fichier << data.dump(3);
}
void addDepence(vector <Depence>& depenses){
    Depence dep;
    cout << "Entrez la description de la depense: ";
    cin.ignore();
    getline(cin , dep.description);
    cout << "Entrez le montant de la depense: ";
    cin >> dep.montant;
    dep.id = depenses.size() +1;
    auto now = chrono::system_clock::now();  // Obtenir l'heure actuelle
     time_t now_c = chrono::system_clock::to_time_t(now);  // Convertir en time_t
    tm* local_time = localtime(&now_c);  // Convertir en structure tm pour manipulation

    // Formater la date au format "YYYY-MM-DD"
    stringstream date_stream;
    date_stream << put_time(local_time, "%Y-%m-%d");  // Formater la date
    string date_str = date_stream.str();  // Convertir en string
    dep.date = date_str;
    depenses.push_back(dep);
    enregistrerDepensesDansFichier(depenses);
    cout << "Depense ajoutee : " << dep.description << " pour " << dep.montant << " montant.\n";
}
void listDepences(vector <Depence>& depenses){
    cout << "ID | Description | Montant | Date \n";
    cout << "-----------------------------------------------\n";
    for(const auto oneDep : depenses){
         cout << oneDep.id << " | " << oneDep.description << " | " << fixed <<setprecision(2)<< oneDep.montant << " DH" << " | " <<oneDep.date <<"\n";
    }
}
void deleteDepence(vector <Depence>& depenses){
    int id;
    bool trouve = false;
    cout << "Entrez l'ID de la depense à supprimer: ";
    cin >> id;
    //chercher par id
    for( int i = 0  ; i<depenses.size() ; i++ ){
        if(depenses[i].id == id){
            depenses.erase(depenses.begin() + i);
            cout << "Depense supprimee avec succes.\n";
            trouve = true;
            enregistrerDepensesDansFichier(depenses); //enregistrer les modification !
            break;
        }
    }
    if(!trouve){
             cout << "Depense avec ID " << id << " non trouvee.\n";
        }
}

void charcherDepencesDepuisFichier(vector <Depence>& depenses){
ifstream fichier("depenses.json"); //lire le fichier.json
    json data;
    fichier >> data;
    for(const auto& item : data){
        //Convertir item en une struct Depence
        Depence dep;
        dep.id = item["id"];
        dep.date = item["date"];
        dep.description= item["description"];
        dep.montant = item["montant"];
        //Ajouter cette struct dep au vecteu
        depenses.push_back(dep);
    }

}
int main()
{

    cout << "==============================================\n";
    cout << "     Bienvenue dans le Gestionnaire de Depenses\n";
    cout << "==============================================\n";

    vector<Depence>depenses;
    charcherDepencesDepuisFichier(depenses);
    char choix;

   while(true){
       afficherIntroduction();
       cout << "Entrer votre choix : ";
        cin >> choix;
     switch(choix){
    case '1':
        addDepence(depenses);
        break;
    case '2':
        listDepences(depenses);
        break;
    case '3':
        deleteDepence(depenses);
        break;
    case '4':
        cout << "Merci pour l'utilisation a bientot ! \n";
        return 0;
    default :
        cout << "Commande invalide ! \n";

    }

   }
    return 0;
}
