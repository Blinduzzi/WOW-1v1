#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Abilitate_Buff{
    public:
    string name;
    int stage_initial, stage;
};
class Abilitate{
    public:
    string name, type_of_attack;
    vector<Abilitate_Buff> self, enemy;
    int PWR = 0;
};

class Caracter{
    public:
    string name, control;
    vector<Abilitate> abilities;
    int level, BASE_HP = 1, BASE_ATK = 1, BASE_DEF = 1, BASE_ATK_SP = 1, BASE_DEF_SP = 1, BASE_SPD = 1;
    int ATK_level, DEF_level, ATK_SP_level, DEF_SP_level, SPD_level;
    int HP, ATK, DEF, ATK_SP, DEF_SP, SPD;
    int ATK_STAGE = 0, DEF_STAGE = 0, ATK_SP_STAGE = 0, DEF_SP_STAGE = 0, SPD_STAGE = 0;
    int STAGE, EXP = 0;
    void updateAttributes(){
        HP = BASE_HP * level / 50 + level + 10;
        ATK = ATK_level = BASE_ATK * level / 50 + 10;
        DEF = DEF_level = BASE_DEF * level / 50 + 10;
        ATK_SP = ATK_SP_level = BASE_ATK_SP * level / 50 + 10;
        DEF_SP = DEF_SP_level = BASE_DEF_SP * level / 50 + 10;
        SPD = SPD_level = BASE_SPD* level / 50 + 10;
    }
    void afisare(){
        cout << HP << " " << ATK << " " << DEF << " " << ATK_SP << " " 
                                        << DEF_SP << " " << SPD << endl;
    }
};

int main(){
    // citesc linie cu linie si pun caracterele in vector
    vector <string> fileNames;
    vector <Caracter> caractere;
    string line;
    int count = 0;
    while(getline(cin, line)){
        if( count < 2){
            fileNames.push_back(line);
        }
        else{ 
            vector<string> tokens;
            stringstream check(line);
            string intermediate;
            while(getline(check, intermediate, ' '))
            {
                tokens.push_back(intermediate);
            }
            Caracter x;
            x.name = tokens[0];
            stringstream lvl(tokens[1]);
            lvl >> x.level;
            x.control = tokens[2];
            for( int i = 3 ; i < tokens.size() ; i++){
                Abilitate y;
                y.name = tokens[i];
                x.abilities.push_back(y);
            }
            caractere.push_back(x);
        }
        count++;
    }
    // caut caracterele in fisier ca sa le iau statusurile
    for( int i = 0 ; i < caractere.size() ; i++){
        ifstream carac(fileNames[0]);
        while(getline(carac, line)){
            vector<string> tokens;
            stringstream check(line);
            string intermediate;
            while(getline(check, intermediate, ' '))
            {
                tokens.push_back(intermediate);
            }
            if(tokens[0] == caractere[i].name){
                stringstream base_hp(tokens[1]);
                stringstream base_atk(tokens[2]);
                stringstream base_def(tokens[3]);
                stringstream base_atk_sp(tokens[4]);
                stringstream base_def_sp(tokens[5]);
                stringstream base_spd(tokens[6]);
                base_hp >> caractere[i].BASE_HP;
                base_atk >> caractere[i].BASE_ATK;
                base_def >> caractere[i].BASE_DEF;
                base_atk_sp >> caractere[i].BASE_ATK_SP;
                base_def_sp >> caractere[i].BASE_DEF_SP;
                base_spd >> caractere[i].BASE_SPD;
            }
        }
        carac.close();
    }
    // caut abilitatile in fisier ca sa vad ce fac fiecare
    ifstream abilit(fileNames[1]);
    while(getline(abilit, line)){
        vector<string> tokens;
        stringstream check(line);
        string intermediate;
        while(getline(check, intermediate, ' ')){
            tokens.push_back(intermediate);
        }
        for( int i = 0 ; i < caractere.size() ; i++){
            for( int j = 0 ; j < caractere[i].abilities.size() ; j++){
                if(tokens[0] == caractere[i].abilities[j].name){
                    for( int k = 1 ; k < tokens.size() ; k++){
                        if(tokens[k] == "DMG"){
                            stringstream pwr(tokens[++k]);
                            pwr >> caractere[i].abilities[j].PWR;
                            caractere[i].abilities[j].type_of_attack = tokens[++k];
                        }
                        if(tokens[k] == "SELF_MOD"){
                            Abilitate_Buff x;
                            x.name = tokens[++k];
                            stringstream stg(tokens[++k]);
                            stg >> x.stage_initial;
                            if(x.stage_initial > 6)
                                x.stage_initial = 6;
                            if(x.stage_initial < -6)
                                x.stage_initial = -6;
                            x.stage = x.stage_initial;
                            caractere[i].abilities[j].self.push_back(x);
                        }
                        if(tokens[k] == "ENEMY_MOD"){
                            Abilitate_Buff x;
                            x.name = tokens[++k];
                            stringstream stg(tokens[++k]);
                            stg >> x.stage_initial;
                            if(x.stage_initial > 6)
                                x.stage_initial = 6;
                            if(x.stage_initial < -6)
                                x.stage_initial = -6;
                            x.stage = x.stage_initial;
                            caractere[i].abilities[j].enemy.push_back(x);
                        }
                    }
                }
            }
        }
    }
    abilit.close();
    //luptele
    int k = 0;
    int experienta = 0;
    vector<int> kk (caractere.size(), 0);
    for( int i = 1 ; i < caractere.size() ; i++){
        Caracter Eu = caractere[0];
        Eu.updateAttributes();
        Eu.afisare();
        Caracter Inamic = caractere[i];
        Inamic.updateAttributes();
        Inamic.afisare();
        while(Eu.HP > 0 && Inamic.HP > 0){
            //daca eu atac primu
            if(Eu.SPD >= Inamic.SPD){
                // daca eu am control usor
                if(Eu.control == "USOR"){
                    int DMG = 0;
                    if(Eu.abilities[0].type_of_attack =="ATK"){
                        DMG = Eu.level * Eu.abilities[0].PWR * Eu.ATK / (60 * Inamic.DEF);
                    }
                    if(Eu.abilities[0].type_of_attack =="SP_ATK"){
                        DMG = Eu.level * Eu.abilities[0].PWR * Eu.ATK_SP / (60 * Inamic.DEF_SP);
                    }
                    //dau damage
                    Inamic.HP -= DMG;
                    // bag buffuri mie
                    for( int j = 0 ; j < Eu.abilities[0].self.size() ; j++){
                        if(Eu.abilities[0].self[j].name =="ATK"){
                            Eu.ATK_STAGE += Eu.abilities[0].self[j].stage;
                            if(Eu.ATK_STAGE > 6)
                                Eu.ATK_STAGE = 6;
                            if(Eu.ATK_STAGE < -6)
                                Eu.ATK_STAGE = -6;
                            if(Eu.ATK_STAGE > 0)
                                Eu.ATK = (2 + Eu.ATK_STAGE) * Eu.ATK_level / 2;
                            else
                                Eu.ATK = 2*Eu.ATK_level / (2 - Eu.ATK_STAGE );
                        }
                        if(Eu.abilities[0].self[j].name =="DEF"){
                            if(Eu.DEF_STAGE > 6)
                                Eu.DEF_STAGE = 6;
                            if(Eu.DEF_STAGE < -6)
                                Eu.DEF_STAGE = -6;
                            Eu.DEF_STAGE += Eu.abilities[0].self[j].stage;
                            if(Eu.DEF_STAGE > 0)
                                Eu.DEF = (2 + Eu.DEF_STAGE) * Eu.DEF_level / 2;
                            else
                                Eu.DEF = 2*Eu.DEF_level / (2 - Eu.DEF_STAGE );
                        }
                        if(Eu.abilities[0].self[j].name =="SP_ATK"){
                            Eu.ATK_SP_STAGE += Eu.abilities[0].self[j].stage;
                            if(Eu.ATK_SP_STAGE > 6)
                                Eu.ATK_SP_STAGE = 6;
                            if(Eu.ATK_SP_STAGE < -6)
                                Eu.ATK_SP_STAGE = -6;
                            if(Eu.ATK_SP_STAGE > 0)
                                Eu.ATK_SP = (2 + Eu.ATK_SP_STAGE) * Eu.ATK_SP_level / 2;
                            else
                                Eu.ATK_SP = 2*Eu.ATK_SP_level / (2 - Eu.ATK_SP_STAGE );
                        }
                        if(Eu.abilities[0].self[j].name =="SP_DEF"){
                            Eu.DEF_SP_STAGE += Eu.abilities[0].self[j].stage;
                            if(Eu.DEF_SP_STAGE > 6)
                                Eu.DEF_SP_STAGE = 6;
                            if(Eu.DEF_SP_STAGE < -6)
                                Eu.DEF_SP_STAGE = -6;
                            if(Eu.DEF_SP_STAGE > 0)
                                Eu.DEF_SP = (2 + Eu.DEF_SP_STAGE) * Eu.DEF_SP_level / 2;
                            else
                                Eu.DEF_SP = 2*Eu.DEF_SP_level / (2 - Eu.DEF_SP_STAGE );
                        }
                        if(Eu.abilities[0].self[j].name =="SPD"){
                            Eu.SPD_STAGE += Eu.abilities[0].self[j].stage;
                            if(Eu.SPD_STAGE > 6)
                                Eu.SPD_STAGE = 6;
                            if(Eu.SPD_STAGE < -6)
                                Eu.SPD_STAGE = -6;
                            if(Eu.SPD_STAGE > 0)
                                Eu.SPD = (2 + Eu.SPD_STAGE) * Eu.SPD_level / 2;
                            else
                                Eu.SPD = 2*Eu.SPD_level / (2 - Eu.SPD_STAGE );
                        }   
                    }
                    //daca il omor gata
                    if(Inamic.HP <= 0){
                        experienta += Inamic.level;
                        while(experienta > 2 * Eu.level){
                            experienta -= 2 * Eu.level;
                            caractere[0].level ++;
                            if( caractere[0].level > 100)
                                caractere[0].level = 100;
                        }
                        Eu.afisare();
                        Inamic.afisare();
                        break;
                    }
                    // bag debuffuri la inamic
                    for( int j = 0 ; j < Eu.abilities[0].enemy.size() ; j++){
                        if(Eu.abilities[0].enemy[j].name =="ATK"){
                            Inamic.ATK_STAGE += Eu.abilities[0].enemy[j].stage;
                            if(Inamic.ATK_STAGE > 6)
                                Inamic.ATK_STAGE = 6;
                            if(Inamic.ATK_STAGE < -6)
                                Inamic.ATK_STAGE = -6;
                            if(Inamic.ATK_STAGE > 0)
                                Inamic.ATK = (2 + Inamic.ATK_STAGE) * Inamic.ATK_level / 2;
                            else
                                Inamic.ATK = 2*Inamic.ATK_level / (2 - Inamic.ATK_STAGE );
                        }
                        if(Eu.abilities[0].enemy[j].name =="DEF"){
                            Inamic.DEF_STAGE += Eu.abilities[0].enemy[j].stage;
                            if(Inamic.DEF_STAGE > 6)
                                Inamic.DEF_STAGE = 6;
                            if(Inamic.DEF_STAGE < -6)
                                Inamic.DEF_STAGE = -6;
                            if(Inamic.DEF_STAGE > 0)
                                Inamic.DEF = (2 + Inamic.DEF_STAGE) * Inamic.DEF_level / 2;
                            else
                                Inamic.DEF = 2*Inamic.DEF_level / (2 - Inamic.DEF_STAGE );
                        }
                        if(Eu.abilities[0].enemy[j].name =="SP_ATK"){
                            Inamic.ATK_SP_STAGE += Eu.abilities[0].enemy[j].stage;
                            if(Inamic.ATK_SP_STAGE > 6)
                                Inamic.ATK_SP_STAGE = 6;
                            if(Inamic.ATK_SP_STAGE < -6)
                                Inamic.ATK_SP_STAGE = -6;
                            if(Inamic.ATK_SP_STAGE > 0)
                                Inamic.ATK_SP = (2 + Inamic.ATK_SP_STAGE) * Inamic.ATK_SP_level / 2;
                            else
                                Inamic.ATK_SP = 2*Inamic.ATK_SP_level / (2 - Inamic.ATK_SP_STAGE );
                        }
                        if(Eu.abilities[0].enemy[j].name =="SP_DEF"){
                            Inamic.DEF_SP_STAGE += Eu.abilities[0].enemy[j].stage;
                            if(Inamic.ATK_SP_STAGE > 6)
                                Inamic.ATK_SP_STAGE = 6;
                            if(Inamic.ATK_SP_STAGE < -6)
                                Inamic.ATK_SP_STAGE = -6;
                            if(Inamic.DEF_SP_STAGE > 0)
                                Inamic.DEF_SP = (2 + Inamic.DEF_SP_STAGE) * Inamic.DEF_SP_level / 2;
                            else
                                Inamic.DEF_SP = 2*Inamic.DEF_SP_level / (2 - Inamic.DEF_SP_STAGE );
                        }
                        if(Eu.abilities[0].enemy[j].name =="SPD"){
                            Inamic.SPD_STAGE += Eu.abilities[0].enemy[j].stage;
                            if(Inamic.SPD_STAGE > 6)
                                Inamic.SPD_STAGE = 6;
                            if(Inamic.SPD_STAGE < -6)
                                Inamic.SPD_STAGE = -6;
                            if(Inamic.SPD_STAGE > 0)
                                Inamic.SPD = (2 + Inamic.SPD_STAGE) * Inamic.SPD_level / 2;
                            else
                                Inamic.SPD = 2*Inamic.SPD_level / (2 - Inamic.SPD_STAGE );
                        }   
                    }
                }
                if(Eu.control == "MEDIU"){
                    int DMG = 0;
                    if(Eu.abilities[k].type_of_attack =="ATK"){
                        DMG = Eu.level * Eu.abilities[k].PWR * Eu.ATK / (60 * Inamic.DEF);
                    }
                    if(Eu.abilities[k].type_of_attack =="SP_ATK"){
                        DMG = Eu.level * Eu.abilities[k].PWR * Eu.ATK_SP / (60 * Inamic.DEF_SP);
                    }
                    //dau damage
                    Inamic.HP -= DMG;
                    // bag buffuri mie
                    for( int j = 0 ; j < Eu.abilities[k].self.size() ; j++){
                        if(Eu.abilities[k].self[j].name =="ATK"){
                            Eu.ATK_STAGE += Eu.abilities[k].self[j].stage;
                            if(Eu.ATK_STAGE > 6)
                                Eu.ATK_STAGE = 6;
                            if(Eu.ATK_STAGE < -6)
                                Eu.ATK_STAGE = -6;
                            if(Eu.ATK_STAGE > 0)
                                Eu.ATK = (2 + Eu.ATK_STAGE) * Eu.ATK_level / 2;
                            else
                                Eu.ATK = 2*Eu.ATK_level / (2 - Eu.ATK_STAGE );
                            
                        }
                        if(Eu.abilities[k].self[j].name =="DEF"){
                            Eu.DEF_STAGE += Eu.abilities[k].self[j].stage;
                            if(Eu.DEF_STAGE > 6)
                                Eu.DEF_STAGE = 6;
                            if(Eu.DEF_STAGE < -6)
                                Eu.DEF_STAGE = -6;
                            if(Eu.DEF_STAGE > 0)
                                Eu.DEF = (2 + Eu.DEF_STAGE) * Eu.DEF_level / 2;
                            else
                                Eu.DEF = 2*Eu.DEF_level / (2 - Eu.DEF_STAGE );
                            
                        }
                        if(Eu.abilities[k].self[j].name =="SP_ATK"){
                            Eu.ATK_SP_STAGE += Eu.abilities[k].self[j].stage;
                            if(Eu.ATK_SP_STAGE > 6)
                                Eu.ATK_SP_STAGE = 6;
                            if(Eu.ATK_SP_STAGE < -6)
                                Eu.ATK_SP_STAGE = -6;
                            if(Eu.ATK_SP_STAGE > 0)
                                Eu.ATK_SP = (2 + Eu.ATK_SP_STAGE) * Eu.ATK_SP_level / 2;
                            else
                                Eu.ATK_SP = 2*Eu.ATK_SP_level / (2 - Eu.ATK_SP_STAGE );
                            
                        }
                        if(Eu.abilities[k].self[j].name =="SP_DEF"){
                            Eu.DEF_SP_STAGE += Eu.abilities[k].self[j].stage;
                            if(Eu.DEF_SP_STAGE > 6)
                                Eu.DEF_SP_STAGE = 6;
                            if(Eu.DEF_SP_STAGE < -6)
                                Eu.DEF_SP_STAGE = -6;
                            if(Eu.DEF_SP_STAGE > 0)
                                Eu.DEF_SP = (2 + Eu.DEF_SP_STAGE) * Eu.DEF_SP_level / 2;
                            else
                                Eu.DEF_SP = 2*Eu.DEF_SP_level / (2 - Eu.DEF_SP_STAGE );
                            
                        }
                        if(Eu.abilities[k].self[j].name =="SPD"){
                            Eu.SPD_STAGE += Eu.abilities[k].self[j].stage;
                            if(Eu.SPD_STAGE > 6)
                                Eu.SPD_STAGE = 6;
                            if(Eu.SPD_STAGE < -6)
                                Eu.SPD_STAGE = -6;
                            if(Eu.SPD_STAGE > 0)
                                Eu.SPD = (2 + Eu.SPD_STAGE) * Eu.SPD_level / 2;
                            else
                                Eu.SPD = 2*Eu.SPD_level / (2 - Eu.SPD_STAGE );
                            
                        }   
                    }
                    //daca il omor gata
                     if(Inamic.HP <= 0){
                        experienta += Inamic.level;
                        while(experienta > 2 * Eu.level){
                            experienta -= 2 * Eu.level;
                            caractere[0].level ++;
                            if( caractere[0].level > 100)
                                caractere[0].level = 100;
                        }
                        Eu.afisare();
                        Inamic.afisare();
                        k++;
                        k = k % Eu.abilities.size();
                        break;
                    }
                    // bag debuffuri la inamic
                    for( int j = 0 ; j < Eu.abilities[k].enemy.size() ; j++){
                        if(Eu.abilities[k].enemy[j].name =="ATK"){
                            Inamic.ATK_STAGE += Eu.abilities[k].enemy[j].stage;
                            if(Inamic.ATK_STAGE > 6)
                                Inamic.ATK_STAGE = 6;
                            if(Inamic.ATK_STAGE < -6)
                                Inamic.ATK_STAGE = -6;
                            if(Inamic.ATK_STAGE > 0)
                                Inamic.ATK = (2 + Inamic.ATK_STAGE) * Inamic.ATK_level / 2;
                            else
                                Inamic.ATK = 2*Inamic.ATK_level / (2 - Inamic.ATK_STAGE );
                            
                        }
                        if(Eu.abilities[k].enemy[j].name =="DEF"){
                            Inamic.DEF_STAGE += Eu.abilities[k].enemy[j].stage;
                            if(Inamic.DEF_STAGE > 6)
                                Inamic.DEF_STAGE = 6;
                            if(Inamic.DEF_STAGE < -6)
                                Inamic.DEF_STAGE = -6;
                            if(Inamic.DEF_STAGE > 0)
                                Inamic.DEF = (2 + Inamic.DEF_STAGE) * Inamic.DEF_level / 2;
                            else
                                Inamic.DEF = 2*Inamic.DEF_level / (2 - Inamic.DEF_STAGE );
                            
                        }
                        if(Eu.abilities[k].enemy[j].name =="SP_ATK"){
                            Inamic.ATK_SP_STAGE += Eu.abilities[k].enemy[j].stage;
                            if(Inamic.ATK_SP_STAGE > 6)
                                Inamic.ATK_SP_STAGE = 6;
                            if(Inamic.ATK_SP_STAGE < -6)
                                Inamic.ATK_SP_STAGE = -6;
                            if(Inamic.ATK_SP_STAGE > 0)
                                Inamic.ATK_SP = (2 + Inamic.ATK_SP_STAGE) * Inamic.ATK_SP_level / 2;
                            else
                                Inamic.ATK_SP = 2*Inamic.ATK_SP_level / (2 - Inamic.ATK_SP_STAGE );
                            
                        }
                        if(Eu.abilities[k].enemy[j].name =="SP_DEF"){
                            Inamic.DEF_SP_STAGE += Eu.abilities[k].enemy[j].stage;
                            if(Inamic.DEF_SP_STAGE > 6)
                                Inamic.DEF_SP_STAGE = 6;
                            if(Inamic.DEF_SP_STAGE < -6)
                                Inamic.DEF_SP_STAGE = -6;
                            if(Inamic.DEF_SP_STAGE > 0)
                                Inamic.DEF_SP = (2 + Inamic.DEF_SP_STAGE) * Inamic.DEF_SP_level / 2;
                            else
                                Inamic.DEF_SP = 2*Inamic.DEF_SP_level / (2 - Inamic.DEF_SP_STAGE );
                            
                        }
                        if(Eu.abilities[k].enemy[j].name =="SPD"){
                            Inamic.SPD_STAGE += Eu.abilities[k].enemy[j].stage;
                            if(Inamic.SPD_STAGE > 6)
                                Inamic.SPD_STAGE = 6;
                            if(Inamic.SPD_STAGE < -6)
                                Inamic.SPD_STAGE = -6;
                            if(Inamic.SPD_STAGE > 0)
                                Inamic.SPD = (2 + Inamic.SPD_STAGE) * Inamic.SPD_level / 2;
                            else
                                Inamic.SPD = 2*Inamic.SPD_level / (2 - Inamic.SPD_STAGE );
                            
                        }   
                    }
                    k++;
                    k = k % Eu.abilities.size();
                }
                if(Inamic.control == "USOR"){
                    int DMG = 0;
                    if(Inamic.abilities[0].type_of_attack =="ATK"){
                        DMG = Inamic.level * Inamic.abilities[0].PWR * Inamic.ATK / (60 * Eu.DEF);
                    }
                    if(Inamic.abilities[0].type_of_attack =="SP_ATK"){
                        DMG = Inamic.level * Inamic.abilities[0].PWR * Inamic.ATK_SP / (60 * Eu.DEF_SP);
                    }
                    //imi da damage
                    Eu.HP -= DMG;
                    // baga buffuri lui
                    for( int j = 0 ; j < Inamic.abilities[0].self.size() ; j++){
                        if(Inamic.abilities[0].self[j].name =="ATK"){
                            Inamic.ATK_STAGE += Inamic.abilities[0].self[j].stage;
                            if(Inamic.ATK_STAGE > 6)
                                Inamic.ATK_STAGE = 6;
                            if(Inamic.ATK_STAGE < -6)
                                Inamic.ATK_STAGE = -6;
                            if(Inamic.ATK_STAGE > 0)
                                Inamic.ATK = (2 + Inamic.ATK_STAGE) * Inamic.ATK_level / 2;
                            else
                                Inamic.ATK = 2*Inamic.ATK_level / (2 - Inamic.ATK_STAGE );
                            
                        }
                        if(Inamic.abilities[0].self[j].name =="DEF"){
                            Inamic.DEF_STAGE += Inamic.abilities[0].self[j].stage;
                            if(Inamic.DEF_STAGE > 6)
                                Inamic.DEF_STAGE = 6;
                            if(Inamic.DEF_STAGE < -6)
                                Inamic.DEF_STAGE = -6;
                            if(Inamic.DEF_STAGE > 0)
                                Inamic.DEF = (2 + Inamic.DEF_STAGE) * Inamic.DEF_level / 2;
                            else
                                Inamic.DEF = 2*Inamic.DEF_level / (2 - Inamic.DEF_STAGE );
                            
                        }
                        if(Inamic.abilities[0].self[j].name =="SP_ATK"){
                            Inamic.ATK_SP_STAGE += Inamic.abilities[0].self[j].stage;
                            if(Inamic.ATK_SP_STAGE > 6)
                                Inamic.ATK_SP_STAGE = 6;
                            if(Inamic.ATK_SP_STAGE < -6)
                                Inamic.ATK_SP_STAGE = -6;
                            if(Inamic.ATK_SP_STAGE > 0)
                                Inamic.ATK_SP = (2 + Inamic.ATK_SP_STAGE) * Inamic.ATK_SP_level / 2;
                            else
                                Inamic.ATK_SP = 2*Inamic.ATK_SP_level / (2 - Inamic.ATK_SP_STAGE );
                            
                        }
                        if(Inamic.abilities[0].self[j].name =="SP_DEF"){
                            Inamic.DEF_SP_STAGE += Inamic.abilities[0].self[j].stage;
                            if(Inamic.DEF_SP_STAGE > 6)
                                Inamic.DEF_SP_STAGE = 6;
                            if(Inamic.DEF_SP_STAGE < -6)
                                Inamic.DEF_SP_STAGE = -6;
                            if(Inamic.DEF_SP_STAGE > 0)
                                Inamic.DEF_SP = (2 + Inamic.DEF_SP_STAGE) * Inamic.DEF_SP_level / 2;
                            else
                                Inamic.DEF_SP = 2*Inamic.DEF_SP_level / (2 - Inamic.DEF_SP_STAGE );
                            
                        }
                        if(Inamic.abilities[0].self[j].name =="SPD"){
                            Inamic.SPD_STAGE += Inamic.abilities[0].self[j].stage;
                            if(Inamic.SPD_STAGE > 6)
                                Inamic.SPD_STAGE = 6;
                            if(Inamic.SPD_STAGE < -6)
                                Inamic.SPD_STAGE = -6;
                            if(Inamic.SPD_STAGE > 0)
                                Inamic.SPD = (2 + Inamic.SPD_STAGE) * Inamic.SPD_level / 2;
                            else
                                Inamic.SPD = 2*Inamic.SPD_level / (2 - Inamic.SPD_STAGE );
                            
                        }   
                    }
                    //daca mor gata
                    if(Eu.HP <= 0){
                        Eu.afisare();
                        Inamic.afisare();
                        break;
                    }
                    // baga debuffuri mie
                    for( int j = 0 ; j < Inamic.abilities[0].enemy.size() ; j++){
                        if(Inamic.abilities[0].enemy[j].name =="ATK"){
                            Eu.ATK_STAGE += Inamic.abilities[0].enemy[j].stage;
                            if(Eu.ATK_STAGE > 6)
                                Eu.ATK_STAGE = 6;
                            if(Eu.ATK_STAGE < -6)
                                Eu.ATK_STAGE = -6;
                            if(Eu.ATK_STAGE > 0)
                                Eu.ATK = (2 + Eu.ATK_STAGE) * Eu.ATK_level / 2;
                            else
                                Eu.ATK = 2*Eu.ATK_level / (2 - Eu.ATK_STAGE );
                            
                        }
                        if(Inamic.abilities[0].enemy[j].name =="DEF"){
                            Eu.DEF_STAGE += Inamic.abilities[0].enemy[j].stage;
                            if(Eu.DEF_STAGE > 6)
                                Eu.DEF_STAGE = 6;
                            if(Eu.DEF_STAGE < -6)
                                Eu.DEF_STAGE = -6;
                            if(Eu.DEF_STAGE > 0)
                                Eu.DEF = (2 + Eu.DEF_STAGE) * Eu.DEF_level / 2;
                            else
                                Eu.DEF = 2*Eu.DEF_level / (2 - Eu.DEF_STAGE );
                            
                        }
                        if(Inamic.abilities[0].enemy[j].name =="SP_ATK"){
                            Eu.ATK_SP_STAGE += Inamic.abilities[0].enemy[j].stage;
                            if(Eu.ATK_SP_STAGE > 6)
                                Eu.ATK_SP_STAGE = 6;
                            if(Eu.ATK_SP_STAGE < -6)
                                Eu.ATK_SP_STAGE = -6;
                            if(Eu.ATK_SP_STAGE > 0)
                                Eu.ATK_SP = (2 + Eu.ATK_SP_STAGE) * Eu.ATK_SP_level / 2;
                            else
                                Eu.ATK_SP = 2*Eu.ATK_SP_level / (2 - Eu.ATK_SP_STAGE );
                            
                        }
                        if(Inamic.abilities[0].enemy[j].name =="SP_DEF"){
                            Eu.DEF_SP_STAGE += Inamic.abilities[0].enemy[j].stage;
                            if(Eu.DEF_SP_STAGE > 6)
                                Eu.DEF_SP_STAGE = 6;
                            if(Eu.DEF_SP_STAGE < -6)
                                Eu.DEF_SP_STAGE = -6;
                            if(Eu.DEF_SP_STAGE > 0)
                                Eu.DEF_SP = (2 + Eu.DEF_SP_STAGE) * Eu.DEF_SP_level / 2;
                            else
                                Eu.DEF_SP = 2*Eu.DEF_SP_level / (2 - Eu.DEF_SP_STAGE );
                            
                        }
                        if(Inamic.abilities[0].enemy[j].name =="SPD"){
                            Eu.SPD_STAGE += Inamic.abilities[0].enemy[j].stage;
                            if(Eu.SPD_STAGE > 6)
                                Eu.SPD_STAGE = 6;
                            if(Eu.SPD_STAGE < -6)
                                Eu.SPD_STAGE = -6;
                            if(Eu.SPD_STAGE > 0)
                                Eu.SPD = (2 + Eu.SPD_STAGE) * Eu.SPD_level / 2;
                            else
                                Eu.SPD = 2*Eu.SPD_level / (2 - Eu.SPD_STAGE );
                            
                        }   
                    }
                }
                if(Inamic.control == "MEDIU"){
                    int DMG = 0;
                    if(Inamic.abilities[kk[i]].type_of_attack =="ATK"){
                        DMG = Inamic.level * Inamic.abilities[kk[i]].PWR * Inamic.ATK / (60 * Inamic.DEF);
                    }
                    if(Inamic.abilities[kk[i]].type_of_attack =="SP_ATK"){
                        DMG = Inamic.level * Inamic.abilities[kk[i]].PWR * Inamic.ATK_SP / (60 * Inamic.DEF_SP);
                    }
                    //da damage
                    Inamic.HP -= DMG;
                    // baga buffuri lui
                    for( int j = 0 ; j < Inamic.abilities[kk[i]].self.size() ; j++){
                        if(Inamic.abilities[kk[i]].self[j].name =="ATK"){
                            Inamic.ATK_STAGE += Inamic.abilities[kk[i]].self[j].stage;
                            if(Inamic.ATK_STAGE > 6)
                                Inamic.ATK_STAGE = 6;
                            if(Inamic.ATK_STAGE < -6)
                                Inamic.ATK_STAGE = -6;
                            if(Inamic.ATK_STAGE > 0)
                                Inamic.ATK = (2 + Inamic.ATK_STAGE) * Inamic.ATK_level / 2;
                            else
                                Inamic.ATK = 2*Inamic.ATK_level / (2 - Inamic.ATK_STAGE );
                            
                        }
                        if(Inamic.abilities[kk[i]].self[j].name =="DEF"){
                            Inamic.DEF_STAGE += Inamic.abilities[kk[i]].self[j].stage;
                            if(Inamic.DEF_STAGE > 6)
                                Inamic.DEF_STAGE = 6;
                            if(Inamic.DEF_STAGE < -6)
                                Inamic.DEF_STAGE = -6;
                            if(Inamic.DEF_STAGE > 0)
                                Inamic.DEF = (2 + Inamic.DEF_STAGE) * Inamic.DEF_level / 2;
                            else
                                Inamic.DEF = 2*Inamic.DEF_level / (2 - Inamic.DEF_STAGE );
                            
                        }
                        if(Inamic.abilities[kk[i]].self[j].name =="SP_ATK"){
                            Inamic.ATK_SP_STAGE += Inamic.abilities[kk[i]].self[j].stage;
                            if(Inamic.ATK_SP_STAGE > 6)
                                Inamic.ATK_SP_STAGE = 6;
                            if(Inamic.ATK_SP_STAGE < -6)
                                Inamic.ATK_SP_STAGE = -6;
                            if(Inamic.ATK_SP_STAGE > 0)
                                Inamic.ATK_SP = (2 + Inamic.ATK_SP_STAGE) * Inamic.ATK_SP_level / 2;
                            else
                                Inamic.ATK_SP = 2*Inamic.ATK_SP_level / (2 - Inamic.ATK_SP_STAGE );
                            
                        }
                        if(Inamic.abilities[kk[i]].self[j].name =="SP_DEF"){
                            Inamic.DEF_SP_STAGE += Inamic.abilities[kk[i]].self[j].stage;
                            if(Inamic.DEF_SP_STAGE > 6)
                                Inamic.DEF_SP_STAGE = 6;
                            if(Inamic.DEF_SP_STAGE < -6)
                                Inamic.DEF_SP_STAGE = -6;
                            if(Inamic.DEF_SP_STAGE > 0)
                                Inamic.DEF_SP = (2 + Inamic.DEF_SP_STAGE) * Inamic.DEF_SP_level / 2;
                            else
                                Inamic.DEF_SP = 2*Inamic.DEF_SP_level / (2 - Inamic.DEF_SP_STAGE );
                            
                        }
                        if(Inamic.abilities[kk[i]].self[j].name =="SPD"){
                            Inamic.SPD_STAGE += Inamic.abilities[kk[i]].self[j].stage;
                            if(Inamic.SPD_STAGE > 6)
                                Inamic.SPD_STAGE = 6;
                            if(Inamic.SPD_STAGE < -6)
                                Inamic.SPD_STAGE = -6;
                            if(Inamic.SPD_STAGE > 0)
                                Inamic.SPD = (2 + Inamic.SPD_STAGE) * Inamic.SPD_level / 2;
                            else
                                Inamic.SPD = 2*Inamic.SPD_level / (2 - Inamic.SPD_STAGE );
                            
                        }   
                    }
                    //daca mor gata
                    if(Eu.HP <= 0){
                        Eu.afisare();
                        Inamic.afisare();
                        kk[i]++;
                        kk[i]= kk[i] % Inamic.abilities.size();
                        break;
                    }
                    // baga debuffuri mie
                    for( int j = 0 ; j < Inamic.abilities[kk[i]].enemy.size() ; j++){
                        if(Inamic.abilities[kk[i]].enemy[j].name =="ATK"){
                            Eu.ATK_STAGE += Inamic.abilities[kk[i]].enemy[j].stage;
                            if(Eu.ATK_STAGE > 6)
                                Eu.ATK_STAGE = 6;
                            if(Eu.ATK_STAGE < -6)
                                Eu.ATK_STAGE = -6;
                            if(Eu.ATK_STAGE > 0)
                                Eu.ATK = (2 + Eu.ATK_STAGE) * Eu.ATK_level / 2;
                            else
                                Eu.ATK = 2*Eu.ATK_level / (2 - Eu.ATK_STAGE );
                            
                        }
                        if(Inamic.abilities[kk[i]].enemy[j].name =="DEF"){
                            Eu.DEF_STAGE += Inamic.abilities[kk[i]].enemy[j].stage;
                            if(Eu.DEF_STAGE > 6)
                                Eu.DEF_STAGE = 6;
                            if(Eu.DEF_STAGE < -6)
                                Eu.DEF_STAGE = -6;
                            if(Eu.DEF_STAGE > 0)
                                Eu.DEF = (2 + Eu.DEF_STAGE) * Eu.DEF_level / 2;
                            else
                                Eu.DEF = 2*Eu.DEF_level / (2 - Eu.DEF_STAGE );
                            
                        }
                        if(Inamic.abilities[kk[i]].enemy[j].name =="SP_ATK"){
                            Eu.ATK_SP_STAGE += Inamic.abilities[kk[i]].enemy[j].stage;
                            if(Eu.ATK_SP_STAGE > 6)
                                Eu.ATK_SP_STAGE = 6;
                            if(Eu.ATK_SP_STAGE < -6)
                                Eu.ATK_SP_STAGE = -6;
                            if(Eu.ATK_SP_STAGE > 0)
                                Eu.ATK_SP = (2 + Eu.ATK_SP_STAGE) * Eu.ATK_SP_level / 2;
                            else
                                Eu.ATK_SP = 2*Eu.ATK_SP_level / (2 - Eu.ATK_SP_STAGE );
                            
                        }
                        if(Inamic.abilities[kk[i]].enemy[j].name =="SP_DEF"){
                            Eu.DEF_SP_STAGE += Inamic.abilities[kk[i]].enemy[j].stage;
                            if(Eu.DEF_SP_STAGE > 6)
                                Eu.DEF_SP_STAGE = 6;
                            if(Eu.DEF_SP_STAGE < -6)
                                Eu.DEF_SP_STAGE = -6;
                            if(Eu.DEF_SP_STAGE > 0)
                                Eu.DEF_SP = (2 + Eu.DEF_SP_STAGE) * Eu.DEF_SP_level / 2;
                            else
                                Eu.DEF_SP = 2*Eu.DEF_SP_level / (2 - Eu.DEF_SP_STAGE );
                            
                        }
                        if(Inamic.abilities[kk[i]].enemy[j].name =="SPD"){
                            Eu.SPD_STAGE += Inamic.abilities[kk[i]].enemy[j].stage;
                            if(Eu.SPD_STAGE > 6)
                                Eu.SPD_STAGE = 6;
                            if(Eu.SPD_STAGE < -6)
                                Eu.SPD_STAGE = -6;
                            if(Eu.SPD_STAGE > 0)
                                Eu.SPD = (2 + Eu.SPD_STAGE) * Eu.SPD_level / 2;
                            else
                                Eu.SPD = 2*Eu.SPD_level / (2 - Eu.SPD_STAGE );
                            
                        }   
                    }
                    kk[i]++;
                    kk[i] = kk[i] % Inamic.abilities.size();
                }
                
            }
            //daca el ataca primu
            else{
                if(Inamic.control == "USOR"){
                    int DMG = 0;
                    if(Inamic.abilities[0].type_of_attack =="ATK"){
                        DMG = Inamic.level * Inamic.abilities[0].PWR * Inamic.ATK / (60 * Eu.DEF);
                    }
                    if(Inamic.abilities[0].type_of_attack =="SP_ATK"){
                        DMG = Inamic.level * Inamic.abilities[0].PWR * Inamic.ATK_SP / (60 * Eu.DEF_SP);
                    }
                    //imi da damage
                    Eu.HP -= DMG;
                    // baga buffuri lui
                    for( int j = 0 ; j < Inamic.abilities[0].self.size() ; j++){
                        if(Inamic.abilities[0].self[j].name =="ATK"){
                            Inamic.ATK_STAGE += Inamic.abilities[0].self[j].stage;
                            if(Inamic.ATK_STAGE > 6)
                                Inamic.ATK_STAGE = 6;
                            if(Inamic.ATK_STAGE < -6)
                                Inamic.ATK_STAGE = -6;
                            if(Inamic.ATK_STAGE > 0)
                                Inamic.ATK = (2 + Inamic.ATK_STAGE) * Inamic.ATK_level / 2;
                            else
                                Inamic.ATK = 2*Inamic.ATK_level / (2 - Inamic.ATK_STAGE );
                            
                        }
                        if(Inamic.abilities[0].self[j].name =="DEF"){
                            Inamic.DEF_STAGE += Inamic.abilities[0].self[j].stage;
                            if(Inamic.DEF_STAGE > 6)
                                Inamic.DEF_STAGE = 6;
                            if(Inamic.DEF_STAGE < -6)
                                Inamic.DEF_STAGE = -6;
                            if(Inamic.DEF_STAGE > 0)
                                Inamic.DEF = (2 + Inamic.DEF_STAGE) * Inamic.DEF_level / 2;
                            else
                                Inamic.DEF = 2*Inamic.DEF_level / (2 - Inamic.DEF_STAGE );
                            
                        }
                        if(Inamic.abilities[0].self[j].name =="SP_ATK"){
                            Inamic.ATK_SP_STAGE += Inamic.abilities[0].self[j].stage;
                            if(Inamic.ATK_SP_STAGE > 6)
                                Inamic.ATK_SP_STAGE = 6;
                            if(Inamic.ATK_SP_STAGE < -6)
                                Inamic.ATK_SP_STAGE = -6;
                            if(Inamic.ATK_SP_STAGE > 0)
                                Inamic.ATK_SP = (2 + Inamic.ATK_SP_STAGE) * Inamic.ATK_SP_level / 2;
                            else
                                Inamic.ATK_SP = 2*Inamic.ATK_SP_level / (2 - Inamic.ATK_SP_STAGE );
                            
                        }
                        if(Inamic.abilities[0].self[j].name =="SP_DEF"){
                            Inamic.DEF_SP_STAGE += Inamic.abilities[0].self[j].stage;
                            if(Inamic.DEF_SP_STAGE > 6)
                                Inamic.DEF_SP_STAGE = 6;
                            if(Inamic.DEF_SP_STAGE < -6)
                                Inamic.DEF_SP_STAGE = -6;
                            if(Inamic.DEF_SP_STAGE > 0)
                                Inamic.DEF_SP = (2 + Inamic.DEF_SP_STAGE) * Inamic.DEF_SP_level / 2;
                            else
                                Inamic.DEF_SP = 2*Inamic.DEF_SP_level / (2 - Inamic.DEF_SP_STAGE );
                            
                        }
                        if(Inamic.abilities[0].self[j].name =="SPD"){
                            Inamic.SPD_STAGE += Inamic.abilities[0].self[j].stage;
                            if(Inamic.SPD_STAGE > 6)
                                Inamic.SPD_STAGE = 6;
                            if(Inamic.SPD_STAGE < -6)
                                Inamic.SPD_STAGE = -6;
                            if(Inamic.SPD_STAGE > 0)
                                Inamic.SPD = (2 + Inamic.SPD_STAGE) * Inamic.SPD_level / 2;
                            else
                                Inamic.SPD = 2*Inamic.SPD_level / (2 - Inamic.SPD_STAGE );
                            
                        }   
                    }
                    //daca mor gata
                    if(Eu.HP <= 0){
                        Eu.afisare();
                        Inamic.afisare();
                        break;
                    }
                    // baga debuffuri mie
                    for( int j = 0 ; j < Inamic.abilities[0].enemy.size() ; j++){
                        if(Inamic.abilities[0].enemy[j].name =="ATK"){
                            Eu.ATK_STAGE += Inamic.abilities[0].enemy[j].stage;
                            if(Eu.ATK_STAGE > 6)
                                Eu.ATK_STAGE = 6;
                            if(Eu.ATK_STAGE < -6)
                                Eu.ATK_STAGE = -6;
                            if(Eu.ATK_STAGE > 0)
                                Eu.ATK = (2 + Eu.ATK_STAGE) * Eu.ATK_level / 2;
                            else
                                Eu.ATK = 2*Eu.ATK_level / (2 - Eu.ATK_STAGE );
                            
                        }
                        if(Inamic.abilities[0].enemy[j].name =="DEF"){
                            Eu.DEF_STAGE += Inamic.abilities[0].enemy[j].stage;
                            if(Eu.DEF_STAGE > 6)
                                Eu.DEF_STAGE = 6;
                            if(Eu.DEF_STAGE < -6)
                                Eu.DEF_STAGE = -6;
                            if(Eu.DEF_STAGE > 0)
                                Eu.DEF = (2 + Eu.DEF_STAGE) * Eu.DEF_level / 2;
                            else
                                Eu.DEF = 2*Eu.DEF_level / (2 - Eu.DEF_STAGE );
                            
                        }
                        if(Inamic.abilities[0].enemy[j].name =="SP_ATK"){
                            Eu.ATK_SP_STAGE += Inamic.abilities[0].enemy[j].stage;
                            if(Eu.ATK_SP_STAGE > 6)
                                Eu.ATK_SP_STAGE = 6;
                            if(Eu.ATK_SP_STAGE < -6)
                                Eu.ATK_SP_STAGE = -6;
                            if(Eu.ATK_SP_STAGE > 0)
                                Eu.ATK_SP = (2 + Eu.ATK_SP_STAGE) * Eu.ATK_SP_level / 2;
                            else
                                Eu.ATK_SP = 2*Eu.ATK_SP_level / (2 - Eu.ATK_SP_STAGE );
                            
                        }
                        if(Inamic.abilities[0].enemy[j].name =="SP_DEF"){
                            Eu.DEF_SP_STAGE += Inamic.abilities[0].enemy[j].stage;
                            if(Eu.DEF_SP_STAGE > 6)
                                Eu.DEF_SP_STAGE = 6;
                            if(Eu.DEF_SP_STAGE < -6)
                                Eu.DEF_SP_STAGE = -6;
                            if(Eu.DEF_SP_STAGE > 0)
                                Eu.DEF_SP = (2 + Eu.DEF_SP_STAGE) * Eu.DEF_SP_level / 2;
                            else
                                Eu.DEF_SP = 2*Eu.DEF_SP_level / (2 - Eu.DEF_SP_STAGE );
                            
                        }
                        if(Inamic.abilities[0].enemy[j].name =="SPD"){
                            Eu.SPD_STAGE += Inamic.abilities[0].enemy[j].stage;
                            if(Eu.SPD_STAGE > 6)
                                Eu.SPD_STAGE = 6;
                            if(Eu.SPD_STAGE < -6)
                                Eu.SPD_STAGE = -6;
                            if(Eu.SPD_STAGE > 0)
                                Eu.SPD = (2 + Eu.SPD_STAGE) * Eu.SPD_level / 2;
                            else
                                Eu.SPD = 2*Eu.SPD_level / (2 - Eu.SPD_STAGE );
                            
                        }   
                    }
                }
                if(Inamic.control == "MEDIU"){
                    int DMG = 0;
                    if(Inamic.abilities[kk[i]].type_of_attack =="ATK"){
                        DMG = Inamic.level * Inamic.abilities[kk[i]].PWR * Inamic.ATK / (60 * Inamic.DEF);
                    }
                    if(Inamic.abilities[kk[i]].type_of_attack =="SP_ATK"){
                        DMG = Inamic.level * Inamic.abilities[kk[i]].PWR * Inamic.ATK_SP / (60 * Inamic.DEF_SP);
                    }
                    //da damage
                    Inamic.HP -= DMG;
                    // baga buffuri lui
                    for( int j = 0 ; j < Inamic.abilities[kk[i]].self.size() ; j++){
                        if(Inamic.abilities[kk[i]].self[j].name =="ATK"){
                            Inamic.ATK_STAGE += Inamic.abilities[kk[i]].self[j].stage;
                            if(Inamic.ATK_STAGE > 6)
                                Inamic.ATK_STAGE = 6;
                            if(Inamic.ATK_STAGE < -6)
                                Inamic.ATK_STAGE = -6;
                            if(Inamic.ATK_STAGE > 0)
                                Inamic.ATK = (2 + Inamic.ATK_STAGE) * Inamic.ATK_level / 2;
                            else
                                Inamic.ATK = 2*Inamic.ATK_level / (2 - Inamic.ATK_STAGE );
                            
                        }
                        if(Inamic.abilities[kk[i]].self[j].name =="DEF"){
                            Inamic.DEF_STAGE += Inamic.abilities[kk[i]].self[j].stage;
                            if(Inamic.DEF_STAGE > 6)
                                Inamic.DEF_STAGE = 6;
                            if(Inamic.DEF_STAGE < -6)
                                Inamic.DEF_STAGE = -6;
                            if(Inamic.DEF_STAGE > 0)
                                Inamic.DEF = (2 + Inamic.DEF_STAGE) * Inamic.DEF_level / 2;
                            else
                                Inamic.DEF = 2*Inamic.DEF_level / (2 - Inamic.DEF_STAGE );
                            
                        }
                        if(Inamic.abilities[kk[i]].self[j].name =="SP_ATK"){
                            Inamic.ATK_SP_STAGE += Inamic.abilities[kk[i]].self[j].stage;
                            if(Inamic.ATK_SP_STAGE > 6)
                                Inamic.ATK_SP_STAGE = 6;
                            if(Inamic.ATK_SP_STAGE < -6)
                                Inamic.ATK_SP_STAGE = -6;
                            if(Inamic.ATK_SP_STAGE > 0)
                                Inamic.ATK_SP = (2 + Inamic.ATK_SP_STAGE) * Inamic.ATK_SP_level / 2;
                            else
                                Inamic.ATK_SP = 2*Inamic.ATK_SP_level / (2 - Inamic.ATK_SP_STAGE );
                            
                        }
                        if(Inamic.abilities[kk[i]].self[j].name =="SP_DEF"){
                            Inamic.DEF_SP_STAGE += Inamic.abilities[kk[i]].self[j].stage;
                            if(Inamic.DEF_SP_STAGE > 6)
                                Inamic.DEF_SP_STAGE = 6;
                            if(Inamic.DEF_SP_STAGE < -6)
                                Inamic.DEF_SP_STAGE = -6;
                            if(Inamic.DEF_SP_STAGE > 0)
                                Inamic.DEF_SP = (2 + Inamic.DEF_SP_STAGE) * Inamic.DEF_SP_level / 2;
                            else
                                Inamic.DEF_SP = 2*Inamic.DEF_SP_level / (2 - Inamic.DEF_SP_STAGE );
                            
                        }
                        if(Inamic.abilities[kk[i]].self[j].name =="SPD"){
                            Inamic.SPD_STAGE += Inamic.abilities[kk[i]].self[j].stage;
                            if(Inamic.SPD_STAGE > 6)
                                Inamic.SPD_STAGE = 6;
                            if(Inamic.SPD_STAGE < -6)
                                Inamic.SPD_STAGE = -6;
                            if(Inamic.SPD_STAGE > 0)
                                Inamic.SPD = (2 + Inamic.SPD_STAGE) * Inamic.SPD_level / 2;
                            else
                                Inamic.SPD = 2*Inamic.SPD_level / (2 - Inamic.SPD_STAGE );
                            
                        }   
                    }
                    //daca mor gata
                    if(Eu.HP <= 0){
                        Eu.afisare();
                        Inamic.afisare();
                        kk[i]++;
                        kk[i]= kk[i] % Inamic.abilities.size();
                        break;
                    }
                    // baga debuffuri mie
                    for( int j = 0 ; j < Inamic.abilities[kk[i]].enemy.size() ; j++){
                        if(Inamic.abilities[kk[i]].enemy[j].name =="ATK"){
                            Eu.ATK_STAGE += Inamic.abilities[kk[i]].enemy[j].stage;
                            if(Eu.ATK_STAGE > 6)
                                Eu.ATK_STAGE = 6;
                            if(Eu.ATK_STAGE < -6)
                                Eu.ATK_STAGE = -6;
                            if(Eu.ATK_STAGE > 0)
                                Eu.ATK = (2 + Eu.ATK_STAGE) * Eu.ATK_level / 2;
                            else
                                Eu.ATK = 2*Eu.ATK_level / (2 - Eu.ATK_STAGE );
                            
                        }
                        if(Inamic.abilities[kk[i]].enemy[j].name =="DEF"){
                            Eu.DEF_STAGE += Inamic.abilities[kk[i]].enemy[j].stage;
                            if(Eu.DEF_STAGE > 6)
                                Eu.DEF_STAGE = 6;
                            if(Eu.DEF_STAGE < -6)
                                Eu.DEF_STAGE = -6;
                            if(Eu.DEF_STAGE > 0)
                                Eu.DEF = (2 + Eu.DEF_STAGE) * Eu.DEF_level / 2;
                            else
                                Eu.DEF = 2*Eu.DEF_level / (2 - Eu.DEF_STAGE );
                            
                        }
                        if(Inamic.abilities[kk[i]].enemy[j].name =="SP_ATK"){
                            Eu.ATK_SP_STAGE += Inamic.abilities[kk[i]].enemy[j].stage;
                            if(Eu.ATK_SP_STAGE > 6)
                                Eu.ATK_SP_STAGE = 6;
                            if(Eu.ATK_SP_STAGE < -6)
                                Eu.ATK_SP_STAGE = -6;
                            if(Eu.ATK_SP_STAGE > 0)
                                Eu.ATK_SP = (2 + Eu.ATK_SP_STAGE) * Eu.ATK_SP_level / 2;
                            else
                                Eu.ATK_SP = 2*Eu.ATK_SP_level / (2 - Eu.ATK_SP_STAGE );
                            
                        }
                        if(Inamic.abilities[kk[i]].enemy[j].name =="SP_DEF"){
                            Eu.DEF_SP_STAGE += Inamic.abilities[kk[i]].enemy[j].stage;
                            if(Eu.DEF_SP_STAGE > 6)
                                Eu.DEF_SP_STAGE = 6;
                            if(Eu.DEF_SP_STAGE < -6)
                                Eu.DEF_SP_STAGE = -6;
                            if(Eu.DEF_SP_STAGE > 0)
                                Eu.DEF_SP = (2 + Eu.DEF_SP_STAGE) * Eu.DEF_SP_level / 2;
                            else
                                Eu.DEF_SP = 2*Eu.DEF_SP_level / (2 - Eu.DEF_SP_STAGE );
                            
                        }
                        if(Inamic.abilities[kk[i]].enemy[j].name =="SPD"){
                            Eu.SPD_STAGE += Inamic.abilities[kk[i]].enemy[j].stage;
                            if(Eu.SPD_STAGE > 6)
                                Eu.SPD_STAGE = 6;
                            if(Eu.SPD_STAGE < -6)
                                Eu.SPD_STAGE = -6;
                            if(Eu.SPD_STAGE > 0)
                                Eu.SPD = (2 + Eu.SPD_STAGE) * Eu.SPD_level / 2;
                            else
                                Eu.SPD = 2*Eu.SPD_level / (2 - Eu.SPD_STAGE );
                            
                        }   
                    }
                    kk[i]++;
                    kk[i] = kk[i] % Inamic.abilities.size();
                }
                if(Eu.control == "USOR"){
                    int DMG = 0;
                    if(Eu.abilities[0].type_of_attack =="ATK"){
                        DMG = Eu.level * Eu.abilities[0].PWR * Eu.ATK / (60 * Inamic.DEF);
                    }
                    if(Eu.abilities[0].type_of_attack =="SP_ATK"){
                        DMG = Eu.level * Eu.abilities[0].PWR * Eu.ATK_SP / (60 * Inamic.DEF_SP);
                    }
                    //dau damage
                    Inamic.HP -= DMG;
                    // bag buffuri mie
                    for( int j = 0 ; j < Eu.abilities[0].self.size() ; j++){
                        if(Eu.abilities[0].self[j].name =="ATK"){
                            Eu.ATK_STAGE += Eu.abilities[0].self[j].stage;
                            if(Eu.ATK_STAGE > 6)
                                Eu.ATK_STAGE = 6;
                            if(Eu.ATK_STAGE < -6)
                                Eu.ATK_STAGE = -6;
                            if(Eu.ATK_STAGE > 0)
                                Eu.ATK = (2 + Eu.ATK_STAGE) * Eu.ATK_level / 2;
                            else
                                Eu.ATK = 2*Eu.ATK_level / (2 - Eu.ATK_STAGE );
                        }
                        if(Eu.abilities[0].self[j].name =="DEF"){
                            if(Eu.DEF_STAGE > 6)
                                Eu.DEF_STAGE = 6;
                            if(Eu.DEF_STAGE < -6)
                                Eu.DEF_STAGE = -6;
                            Eu.DEF_STAGE += Eu.abilities[0].self[j].stage;
                            if(Eu.DEF_STAGE > 0)
                                Eu.DEF = (2 + Eu.DEF_STAGE) * Eu.DEF_level / 2;
                            else
                                Eu.DEF = 2*Eu.DEF_level / (2 - Eu.DEF_STAGE );
                        }
                        if(Eu.abilities[0].self[j].name =="SP_ATK"){
                            Eu.ATK_SP_STAGE += Eu.abilities[0].self[j].stage;
                            if(Eu.ATK_SP_STAGE > 6)
                                Eu.ATK_SP_STAGE = 6;
                            if(Eu.ATK_SP_STAGE < -6)
                                Eu.ATK_SP_STAGE = -6;
                            if(Eu.ATK_SP_STAGE > 0)
                                Eu.ATK_SP = (2 + Eu.ATK_SP_STAGE) * Eu.ATK_SP_level / 2;
                            else
                                Eu.ATK_SP = 2*Eu.ATK_SP_level / (2 - Eu.ATK_SP_STAGE );
                        }
                        if(Eu.abilities[0].self[j].name =="SP_DEF"){
                            Eu.DEF_SP_STAGE += Eu.abilities[0].self[j].stage;
                            if(Eu.DEF_SP_STAGE > 6)
                                Eu.DEF_SP_STAGE = 6;
                            if(Eu.DEF_SP_STAGE < -6)
                                Eu.DEF_SP_STAGE = -6;
                            if(Eu.DEF_SP_STAGE > 0)
                                Eu.DEF_SP = (2 + Eu.DEF_SP_STAGE) * Eu.DEF_SP_level / 2;
                            else
                                Eu.DEF_SP = 2*Eu.DEF_SP_level / (2 - Eu.DEF_SP_STAGE );
                        }
                        if(Eu.abilities[0].self[j].name =="SPD"){
                            Eu.SPD_STAGE += Eu.abilities[0].self[j].stage;
                            if(Eu.SPD_STAGE > 6)
                                Eu.SPD_STAGE = 6;
                            if(Eu.SPD_STAGE < -6)
                                Eu.SPD_STAGE = -6;
                            if(Eu.SPD_STAGE > 0)
                                Eu.SPD = (2 + Eu.SPD_STAGE) * Eu.SPD_level / 2;
                            else
                                Eu.SPD = 2*Eu.SPD_level / (2 - Eu.SPD_STAGE );
                        }   
                    }
                    //daca il omor gata
                    if(Inamic.HP <= 0){
                        experienta += Inamic.level;
                        while(experienta > 2 * Eu.level){
                            experienta -= 2 * Eu.level;
                            if( caractere[0].level > 100)
                                caractere[0].level = 100;
                            caractere[0].level ++;
                        }
                        Eu.afisare();
                        Inamic.afisare();
                        break;
                    }
                    // bag debuffuri la inamic
                    for( int j = 0 ; j < Eu.abilities[0].enemy.size() ; j++){
                        if(Eu.abilities[0].enemy[j].name =="ATK"){
                            Inamic.ATK_STAGE += Eu.abilities[0].enemy[j].stage;
                            if(Inamic.ATK_STAGE > 6)
                                Inamic.ATK_STAGE = 6;
                            if(Inamic.ATK_STAGE < -6)
                                Inamic.ATK_STAGE = -6;
                            if(Inamic.ATK_STAGE > 0)
                                Inamic.ATK = (2 + Inamic.ATK_STAGE) * Inamic.ATK_level / 2;
                            else
                                Inamic.ATK = 2*Inamic.ATK_level / (2 - Inamic.ATK_STAGE );
                        }
                        if(Eu.abilities[0].enemy[j].name =="DEF"){
                            Inamic.DEF_STAGE += Eu.abilities[0].enemy[j].stage;
                            if(Inamic.DEF_STAGE > 6)
                                Inamic.DEF_STAGE = 6;
                            if(Inamic.DEF_STAGE < -6)
                                Inamic.DEF_STAGE = -6;
                            if(Inamic.DEF_STAGE > 0)
                                Inamic.DEF = (2 + Inamic.DEF_STAGE) * Inamic.DEF_level / 2;
                            else
                                Inamic.DEF = 2*Inamic.DEF_level / (2 - Inamic.DEF_STAGE );
                        }
                        if(Eu.abilities[0].enemy[j].name =="SP_ATK"){
                            Inamic.ATK_SP_STAGE += Eu.abilities[0].enemy[j].stage;
                            if(Inamic.ATK_SP_STAGE > 6)
                                Inamic.ATK_SP_STAGE = 6;
                            if(Inamic.ATK_SP_STAGE < -6)
                                Inamic.ATK_SP_STAGE = -6;
                            if(Inamic.ATK_SP_STAGE > 0)
                                Inamic.ATK_SP = (2 + Inamic.ATK_SP_STAGE) * Inamic.ATK_SP_level / 2;
                            else
                                Inamic.ATK_SP = 2*Inamic.ATK_SP_level / (2 - Inamic.ATK_SP_STAGE );
                        }
                        if(Eu.abilities[0].enemy[j].name =="SP_DEF"){
                            Inamic.DEF_SP_STAGE += Eu.abilities[0].enemy[j].stage;
                            if(Inamic.ATK_SP_STAGE > 6)
                                Inamic.ATK_SP_STAGE = 6;
                            if(Inamic.ATK_SP_STAGE < -6)
                                Inamic.ATK_SP_STAGE = -6;
                            if(Inamic.DEF_SP_STAGE > 0)
                                Inamic.DEF_SP = (2 + Inamic.DEF_SP_STAGE) * Inamic.DEF_SP_level / 2;
                            else
                                Inamic.DEF_SP = 2*Inamic.DEF_SP_level / (2 - Inamic.DEF_SP_STAGE );
                        }
                        if(Eu.abilities[0].enemy[j].name =="SPD"){
                            Inamic.SPD_STAGE += Eu.abilities[0].enemy[j].stage;
                            if(Inamic.SPD_STAGE > 6)
                                Inamic.SPD_STAGE = 6;
                            if(Inamic.SPD_STAGE < -6)
                                Inamic.SPD_STAGE = -6;
                            if(Inamic.SPD_STAGE > 0)
                                Inamic.SPD = (2 + Inamic.SPD_STAGE) * Inamic.SPD_level / 2;
                            else
                                Inamic.SPD = 2*Inamic.SPD_level / (2 - Inamic.SPD_STAGE );
                        }   
                    }
                }
                if(Eu.control == "MEDIU"){
                    int DMG = 0;
                    if(Eu.abilities[k].type_of_attack =="ATK"){
                        DMG = Eu.level * Eu.abilities[k].PWR * Eu.ATK / (60 * Inamic.DEF);
                    }
                    if(Eu.abilities[k].type_of_attack =="SP_ATK"){
                        DMG = Eu.level * Eu.abilities[k].PWR * Eu.ATK_SP / (60 * Inamic.DEF_SP);
                    }
                    //dau damage
                    Inamic.HP -= DMG;
                    // bag buffuri mie
                    for( int j = 0 ; j < Eu.abilities[k].self.size() ; j++){
                        if(Eu.abilities[k].self[j].name =="ATK"){
                            Eu.ATK_STAGE += Eu.abilities[k].self[j].stage;
                            if(Eu.ATK_STAGE > 6)
                                Eu.ATK_STAGE = 6;
                            if(Eu.ATK_STAGE < -6)
                                Eu.ATK_STAGE = -6;
                            if(Eu.ATK_STAGE > 0)
                                Eu.ATK = (2 + Eu.ATK_STAGE) * Eu.ATK_level / 2;
                            else
                                Eu.ATK = 2*Eu.ATK_level / (2 - Eu.ATK_STAGE );
                            
                        }
                        if(Eu.abilities[k].self[j].name =="DEF"){
                            Eu.DEF_STAGE += Eu.abilities[k].self[j].stage;
                            if(Eu.DEF_STAGE > 6)
                                Eu.DEF_STAGE = 6;
                            if(Eu.DEF_STAGE < -6)
                                Eu.DEF_STAGE = -6;
                            if(Eu.DEF_STAGE > 0)
                                Eu.DEF = (2 + Eu.DEF_STAGE) * Eu.DEF_level / 2;
                            else
                                Eu.DEF = 2*Eu.DEF_level / (2 - Eu.DEF_STAGE );
                            
                        }
                        if(Eu.abilities[k].self[j].name =="SP_ATK"){
                            Eu.ATK_SP_STAGE += Eu.abilities[k].self[j].stage;
                            if(Eu.ATK_SP_STAGE > 6)
                                Eu.ATK_SP_STAGE = 6;
                            if(Eu.ATK_SP_STAGE < -6)
                                Eu.ATK_SP_STAGE = -6;
                            if(Eu.ATK_SP_STAGE > 0)
                                Eu.ATK_SP = (2 + Eu.ATK_SP_STAGE) * Eu.ATK_SP_level / 2;
                            else
                                Eu.ATK_SP = 2*Eu.ATK_SP_level / (2 - Eu.ATK_SP_STAGE );
                            
                        }
                        if(Eu.abilities[k].self[j].name =="SP_DEF"){
                            Eu.DEF_SP_STAGE += Eu.abilities[k].self[j].stage;
                            if(Eu.DEF_SP_STAGE > 6)
                                Eu.DEF_SP_STAGE = 6;
                            if(Eu.DEF_SP_STAGE < -6)
                                Eu.DEF_SP_STAGE = -6;
                            if(Eu.DEF_SP_STAGE > 0)
                                Eu.DEF_SP = (2 + Eu.DEF_SP_STAGE) * Eu.DEF_SP_level / 2;
                            else
                                Eu.DEF_SP = 2*Eu.DEF_SP_level / (2 - Eu.DEF_SP_STAGE );
                            
                        }
                        if(Eu.abilities[k].self[j].name =="SPD"){
                            Eu.SPD_STAGE += Eu.abilities[k].self[j].stage;
                            if(Eu.SPD_STAGE > 6)
                                Eu.SPD_STAGE = 6;
                            if(Eu.SPD_STAGE < -6)
                                Eu.SPD_STAGE = -6;
                            if(Eu.SPD_STAGE > 0)
                                Eu.SPD = (2 + Eu.SPD_STAGE) * Eu.SPD_level / 2;
                            else
                                Eu.SPD = 2*Eu.SPD_level / (2 - Eu.SPD_STAGE );
                            
                        }   
                    }
                    //daca il omor gata
                     if(Inamic.HP <= 0){
                        experienta += Inamic.level;
                        while(experienta > 2 * Eu.level){
                            experienta -= 2 * Eu.level;
                            caractere[0].level ++;
                            if( caractere[0].level > 100)
                                caractere[0].level = 100;
                        }
                        Eu.afisare();
                        Inamic.afisare();
                        k++;
                        k = k % Eu.abilities.size();
                        break;
                    }
                    // bag debuffuri la inamic
                    for( int j = 0 ; j < Eu.abilities[k].enemy.size() ; j++){
                        if(Eu.abilities[k].enemy[j].name =="ATK"){
                            Inamic.ATK_STAGE += Eu.abilities[k].enemy[j].stage;
                            if(Inamic.ATK_STAGE > 6)
                                Inamic.ATK_STAGE = 6;
                            if(Inamic.ATK_STAGE < -6)
                                Inamic.ATK_STAGE = -6;
                            if(Inamic.ATK_STAGE > 0)
                                Inamic.ATK = (2 + Inamic.ATK_STAGE) * Inamic.ATK_level / 2;
                            else
                                Inamic.ATK = 2*Inamic.ATK_level / (2 - Inamic.ATK_STAGE );
                            
                        }
                        if(Eu.abilities[k].enemy[j].name =="DEF"){
                            Inamic.DEF_STAGE += Eu.abilities[k].enemy[j].stage;
                            if(Inamic.DEF_STAGE > 6)
                                Inamic.DEF_STAGE = 6;
                            if(Inamic.DEF_STAGE < -6)
                                Inamic.DEF_STAGE = -6;
                            if(Inamic.DEF_STAGE > 0)
                                Inamic.DEF = (2 + Inamic.DEF_STAGE) * Inamic.DEF_level / 2;
                            else
                                Inamic.DEF = 2*Inamic.DEF_level / (2 - Inamic.DEF_STAGE );
                            
                        }
                        if(Eu.abilities[k].enemy[j].name =="SP_ATK"){
                            Inamic.ATK_SP_STAGE += Eu.abilities[k].enemy[j].stage;
                            if(Inamic.ATK_SP_STAGE > 6)
                                Inamic.ATK_SP_STAGE = 6;
                            if(Inamic.ATK_SP_STAGE < -6)
                                Inamic.ATK_SP_STAGE = -6;
                            if(Inamic.ATK_SP_STAGE > 0)
                                Inamic.ATK_SP = (2 + Inamic.ATK_SP_STAGE) * Inamic.ATK_SP_level / 2;
                            else
                                Inamic.ATK_SP = 2*Inamic.ATK_SP_level / (2 - Inamic.ATK_SP_STAGE );
                            
                        }
                        if(Eu.abilities[k].enemy[j].name =="SP_DEF"){
                            Inamic.DEF_SP_STAGE += Eu.abilities[k].enemy[j].stage;
                            if(Inamic.DEF_SP_STAGE > 6)
                                Inamic.DEF_SP_STAGE = 6;
                            if(Inamic.DEF_SP_STAGE < -6)
                                Inamic.DEF_SP_STAGE = -6;
                            if(Inamic.DEF_SP_STAGE > 0)
                                Inamic.DEF_SP = (2 + Inamic.DEF_SP_STAGE) * Inamic.DEF_SP_level / 2;
                            else
                                Inamic.DEF_SP = 2*Inamic.DEF_SP_level / (2 - Inamic.DEF_SP_STAGE );
                            
                        }
                        if(Eu.abilities[k].enemy[j].name =="SPD"){
                            Inamic.SPD_STAGE += Eu.abilities[k].enemy[j].stage;
                            if(Inamic.SPD_STAGE > 6)
                                Inamic.SPD_STAGE = 6;
                            if(Inamic.SPD_STAGE < -6)
                                Inamic.SPD_STAGE = -6;
                            if(Inamic.SPD_STAGE > 0)
                                Inamic.SPD = (2 + Inamic.SPD_STAGE) * Inamic.SPD_level / 2;
                            else
                                Inamic.SPD = 2*Inamic.SPD_level / (2 - Inamic.SPD_STAGE );
                            
                        }   
                    }
                    k++;
                    k = k % Eu.abilities.size();
                }
            }
            Eu.afisare();
            Inamic.afisare();
        }
        cout << endl;
    }
    return 0;
}
/*
wow.txt
test.txt
TEST_60_WOW 50 USOR EXTREME_SPEED OVERHEAT
TEST_60_WOW 50 USOR OVERHEAT EXTREME_SPEED
*/