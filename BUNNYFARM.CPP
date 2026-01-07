#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <algorithm>
#include <fstream>
using namespace std;

class Bunny{
    public:
    string sex;
    string colour;
    bool mutation = false;
    int age;
    string name;
    bool Vamp;
    string decideSex(){
        sex = (rand()%2 ==0) ? "Male" : "Female";
        return sex;
    }
        
    //don't need to pass through variable vamp since already passed through in class
    void isVamp(){
        int chance = rand() % 100 + 1;
        if (chance <= 2){
            Vamp = true;
        }
        else{
            Vamp = false;
        }
    }
    
    //initializing constructor inside class to make bunny
    Bunny(const string&motherColour,const vector<string>&maleNames,const vector<string>&femaleNames,const vector<string>&vampireNames, const vector<string>&colourlist){
        decideSex();
        isVamp();
        int mutationR = rand()%100;
        if (mutationR < 90){
            colour = motherColour;
            mutation = false;
        }
        else{
            colour = colourlist[rand()%colourlist.size()];
            mutation = true;
        }
        age = 0;
        if (Vamp == true){
            name = vampireNames[rand()%vampireNames.size()];
        }
        else if (sex=="Male"){
            name = maleNames[rand()%maleNames.size()];
        }
        else if (sex == "Female"){
            name = femaleNames[rand()%femaleNames.size()];
        }
                 
        
    }
    
};
enum SeasonEvent {
    None,
    HarshWinter,
    AbundantSpring,
    PredatorSeason,
    Eclipse,
    Plague
};
class Farm{
    private:
    vector<string> diseaseNames = {
        "Verdant Rot", "Rootblight Syndrome", "Leaffade Affliction",
        "Thornspore Malaise", "Willowbane Fever", "Moldmire Infection",
        "Grove Rattle", "Wildbloom Languish", "Sapburn Decay", "Petal Wasting",
        "Moonshadow Pox", "Starfall Consumption", "Wisp Rot", "Silverfade Malady",
        "Eclipse Pox", "Hollowlight Plague", "Lantern Veil Fever",
        "Astral Leap Sickness", "Dreamtongue Malady", "Spirit Hollow Disorder",
        "V-23 Hop Virus", "Neuro-Whisker Degeneration", "Burrow Neuropathy",
        "Ocular Drift Syndrome", "Spinal Hop Collapse", "Cranial Fade Virus",
        "Systemic Lag Fever", "Tissue Instability Event", "Metabolic Collapse Disease",
        "Respiratory Hop Decline", "Blackfur Marrow Disease", "Crimson Burrow Fever",
        "Bloodveil Plague", "Nightcough Malady", "Sablemane Rot",
        "Shadowbone Infection", "Palegrave Affliction", "Ironlung Consumption",
        "Redthorn Necrosis", "Spectral Withering", "Fluffmelt Pox",
        "Sniffles Gone Bad", "Hopstop Syndrome", "Burrow Drip Fever",
        "Nibble Rot", "Whisker Wilt", "Ear-Tip Tremors", "Tail-Thump Collapse",
        "Cottonmuzzle Infestation", "Softpaw Sickness",
        "The Year of Quiet Hops", "The Second Burrow Collapse",
        "The Great Silver Blight", "The Warren Nightfall",
        "The Moon-Fever Wave", "The Silent Warren Plague",
        "The Year of Burning Grass", "The Burrowfall Mourning",
        "The Red Soil Event", "The Night of Ten Thousand Whiskers"
    };
    bool droughtActive = false;
    int plagueYearsRemaining = 0;
    int plagueCooldownYears = 0;
    float difficultyFactor() {
        // scales from ~0.1 early to ~1.0 late
        float yearFactor = min(1.0, year / 100.0);     // after 50 years it's full difficulty
        float popFactor  = min(1.0, bunnies.size() / 300.0); // full difficulty once 300 bunnies exist

        return (0.1 + 0.9 * ((yearFactor + popFactor) / 2.0));
    }
    void chooseSeasonEvent() {
        int roll = rand() % 100;  // 0–99 random

        if (roll < 50) {
            currentEvent = None;
            logLine("This year passes quietly in the meadow...");
        }
        else if (roll < 70) {
            currentEvent = HarshWinter;
            logLine("❄ A harsh winter sweeps the land — survival becomes difficult.");
        }
        else if (roll < 85) {
            currentEvent = AbundantSpring;
            logLine("🌼 Abundant spring blooms — food and life thrive!");
        }
        else if (roll < 95) {
            currentEvent = PredatorSeason;
            logLine("🦊 Predators are on the prowl — danger lurks everywhere.");
        }
        else {
            currentEvent = Eclipse;
            logLine("🌑 A dark eclipse rises — vampire influence grows...");
        }
        
    }
    
    void applyPredatorDeaths(int kills){
        while (kills > 0 && !bunnies.empty()){
            int idx = rand() % bunnies.size();
            logLine("A predator hunted " + bunnies[idx].name + "!");
            bunnies.erase(bunnies.begin() + idx);
            kills--;
        }
    }
    
    void applyWinterDeaths(float winterSeverity){
        int num = bunnies.size() * winterSeverity;
        for(int i = 0; i < num && !bunnies.empty(); i++){
            int idx = rand() % bunnies.size();
            logLine(bunnies[idx].name + " did not survive the harsh winter.");
            bunnies.erase(bunnies.begin() + idx);
        }
    }
    //seasonal events - have chance for seasonal events to occur                           
    void seasonalEvents() {

        float factor = difficultyFactor();  // difficulty scaling

        // ---- HARSH WINTER ----
        if (rand() % 100 < 2.0 * factor) {
            logLine("❄ A harsh winter strikes the region!");

            float winterSeverity = factor * 0.04; // up to 15% wipeout
            applyWinterDeaths(winterSeverity);
        }

        // ---- PREDATOR SEASON ----
        if (rand() % 100 < 1.0 * factor) {
            logLine("🦊 Predators are on the prowl!");

            int kills = max(1, int(bunnies.size() * factor * 0.01)); // 3% pop damage
            applyPredatorDeaths(kills);
        }

        // ---- DROUGHT ----
        if (!droughtActive && rand() % 200 < 1.0 * factor) {
            applyDroughtPenalty();
        }
        if (plagueYearsRemaining == 0 && rand() % 150 < 1.0 * factor) {

            string disease = diseaseNames[rand() % diseaseNames.size()];
            logLine("☣️ A plague known as " + disease + " breaks out!");
            applyPlague(disease);

            plagueCooldownYears = 5;   // plague cannot happen again for 5 years
        }
    }
    void applyDroughtPenalty(){
        logLine("A drought is reducing fertility this season.");
        droughtActive = true;
    }
    void applyPlague(const string& diseaseName) {
        if (bunnies.empty()) return;

        float factor = difficultyFactor();  
        // kills 2% early game, up to ~20% late game
        float mortalityRate = 0.02 + 0.18 * factor;

        int expectedDeaths = int(bunnies.size() * mortalityRate);
        expectedDeaths = max(1, expectedDeaths);

        logLine("⚠️ " + diseaseName + " intensifies — estimated casualties: " + to_string(expectedDeaths));

        for (int i = 0; i < expectedDeaths && !bunnies.empty(); i++) {
            int idx = rand() % bunnies.size();

            // vampires resist — 50% chance they don't die
            if (bunnies[idx].Vamp && rand() % 2 == 0) {
                logLine("🩸 " + bunnies[idx].name + " resists the plague.");
                continue;  
            }

            logLine("☠️ " + diseaseName + " claimed " + bunnies[idx].name + "...");
            bunnies.erase(bunnies.begin() + idx);
        }
        if (rand() % 100 < 5) {
            int idx = rand() % bunnies.size();
            logLine("✨ " + bunnies[idx].name + " survived " + diseaseName + " and gained immunity!");
            bunnies[idx].colour = "Silver"; // or special tag
        }
    }
    public:
    vector<Bunny> bunnies;
    vector<string> maleNames;
    vector<string> femaleNames;
    vector<string> vampireNames;
    vector<string> colourlist;
    int year = 0;
    SeasonEvent currentEvent = None;
    int delay = 500;
    ofstream logfile;
    
    Farm(const vector<string>&maleNames,const vector<string>&femaleNames,const vector<string>&vampireNames, const vector<string>&colourlist,int delay){
        this->delay = delay;
        this->maleNames = maleNames;
        this->femaleNames = femaleNames;
        this->vampireNames = vampireNames;
        this->colourlist = colourlist;
        for (int i = 0; i < 5; i++){
            string startColour = colourlist[rand()%colourlist.size()];
            Bunny b(startColour, maleNames, femaleNames, vampireNames, colourlist);
            bunnies.push_back(b);
            
        }
        logfile.open("Bunny_Farm_Simulation.txt", ios::trunc);
        
    }
    
    void logLine(const string& msg){
        cout << msg << endl << endl;
        logfile << msg << endl << endl;
        logfile.flush();
    }
    
    void printfarm(){
        for (const Bunny&b : bunnies){
            if (!b.Vamp){
                logLine("Name: " + b.name + ", Sex: " + b.sex + ", Colour: " + b.colour + ", Age: " + to_string(b.age));
            }
            else{
                logLine("Name: " + b.name + ", Sex: Mutant Vampire, Colour: " + b.colour + ", Age: " + to_string(b.age));
            }
            this_thread::sleep_for(chrono::milliseconds(delay));
        }
    }
    
    bool shouldDie(const Bunny&b){
        int roll = rand()%100 +1;
        float overcrowdFactor = 0.0;
        if (bunnies.size() > 100) {
            int over = bunnies.size() - 100;
            overcrowdFactor = over / 100.0;           // e.g., 120 = +0.20
            if (overcrowdFactor > 0.5)                
                overcrowdFactor = 0.5;                // cap at +50%
        }

        int threshold;
        
        if (b.Vamp == false){
            if (b.age <=1){
                threshold =5;
            }
            else if (b.age <=3){
                threshold = 10;
            }
            else if (b.age <=5){
                threshold = 15;
            }
            else if (b.age <=7){
                threshold =30;
            }
            else if (b.age <=9){
                threshold =50;
            }
            else if (b.age <= 10){
                threshold =70;
            }
            else if (b.age <= 12){
                threshold =75;
            }
            else{
                threshold =90;
            }
        
        }
        else{
            if(b.age <=10){
                threshold = 15;
            }
            else if (b.age <=20){
                threshold = 25;
            }
            else if (b.age <= 30){
                threshold =35;
            }
            else if (b.age <=45){
                threshold = 55;
            }
            else if (b.age <=65){
                threshold = 70;
            }
            else if (b.age <=75){
                threshold = 85;
            }
            else if (b.age <=100){
                threshold = 95;
            }
            else{
                threshold =98;
            }

        }
        threshold = 0.5*threshold*((1.0 + overcrowdFactor));
        if (threshold > 100){ 
            threshold = 100;
        }
        if (currentEvent == HarshWinter){
            threshold = min(100, threshold + 10);  // winter kills more
        }

        if (currentEvent == PredatorSeason){
            threshold = min(100, threshold + 15);
        }
        if (currentEvent == Plague){
            threshold = min(100,threshold+25);
        }
        if (b.colour == "Silver") threshold *= 0.5;
        return roll <= threshold;
        
        
    }
                               
                            
    void update(){
        int birthsThisYear = 0;
        int deathsThisYear = 0;
        if (plagueCooldownYears > 0) {
            plagueCooldownYears--;  // one year closer to being allowed again
        }
        chooseSeasonEvent();
        for (Bunny&b : bunnies){
            
            //need to reference instance of bunny's age not actually age variable itself
            b.age+=1;

        }
        seasonalEvents();
        
        vector<Bunny> survivors;
        vector<string> deathreasons = {
            "old age catching up with them",
            "a mysterious illness taking hold",
            "their curiosity leading them astray",
            "vanishing without a trace",
            "the meadow growing quiet around them",
            "the frost finally settling over their trail",
            "winter winds carrying them away",
            "the summer heat proving too tiring",
            "getting swept up in autumn’s quiet decline",
            "the first snow being more than they could bear",
            "a long drought taking its toll",
            "their paws growing too tired for spring",
            "slipping softly into the quiet",
            "their story gently closing",
            "their heartbeat choosing rest",
            "their hop finding its final landing",
            "drifting beyond the meadow’s edge",
            "laying down their burden for good",
            "their spirit bounding toward peace",
            "the stars calling their name",
            "falling asleep beneath the moonlight",
            "joining the sky’s silent choir",
            "a calm stillness embracing them",
            "wandering where other bunnies couldn’t follow",
            "traveling too far into the unknown",
            "losing themselves chasing a horizon",
            "slipping over a hill never returned from",
            "setting off on one last exploration",
            "their curiosity carrying them away",
            "their paws carrying them beyond reach",
            "a mysterious whisker-spark sending them beyond",
            "their myth beginning where their body ended",
            "vanishing into a glowing mist",
            "ascending into whispered folklore",
            "passing through a hidden burrow portal",
            "becoming a guardian spirit of the meadow",
            "disappearing when a strange bell tolled",
            "their last hop landing in eternity",
            "the night guiding them gently home",
            "a final sigh beneath falling petals",
            "quietness finally claiming them"
        };
        
        for (const Bunny&b : bunnies){
            if (!shouldDie(b)){
                survivors.push_back(b);
            }
            else{
                logLine( "Bunny " + b.name + " died from " + deathreasons[rand()%deathreasons.size()] );
                deathsThisYear++;
            }
        }
        bunnies = survivors;
        delay = max(300, 1000 - (int)bunnies.size() * 2);
        bool maleEligible = false;
        for (const Bunny&b : bunnies){
            if (b.sex == "Male" && b.age >=2 && b.Vamp == false){
                maleEligible = true;
                break;
            }
        }
        
        vector<string>mothersColours;
        for (Bunny&b : bunnies){
            if (b.sex == "Female" && maleEligible == true && b.age >=2 && b.Vamp == false){
                mothersColours.push_back(b.colour);
                
            }
        }
        
        
        if (bunnies.size() < 5 && rand()%100 < 10) {
            logLine("🐰 A wandering rabbit arrives!");
            this_thread::sleep_for(chrono::milliseconds(delay));
            string wandererColour = colourlist[rand() % colourlist.size()];
            Bunny newcomer(wandererColour, maleNames, femaleNames, vampireNames, colourlist);
            bunnies.push_back(newcomer);
        }
        
        // --- NEW BREEDING LOGIC — litters instead of expectedBirths math ---

        for (const string& momColour : mothersColours) {

            // Base litter size 0–2
            int litterSize = rand() % 5+3;  

            // Seasonal bonus: in AbundantSpring, give 0–2 more
            if (currentEvent == AbundantSpring) {
                litterSize += rand() % 3;
            }

            // Difficulty scaling — higher difficulty => more bunnies
            litterSize = (0.5*difficultyFactor())*litterSize + (litterSize * difficultyFactor());
            if (litterSize < 0){
                litterSize = 0;
            }

            // Safety cap
            if (litterSize > 12){
                litterSize = 12;
            }
            // Create babies
            for (int i = 0; i < litterSize; i++) {
                Bunny baby(momColour, maleNames, femaleNames, vampireNames, colourlist);
                bunnies.push_back(baby);

                logLine("Bunny " + baby.name + " was born!");

                if (baby.mutation) {
                    logLine("Baby " + baby.name + " has a rare colour: " + baby.colour);
                }

                birthsThisYear++;
                this_thread::sleep_for(chrono::milliseconds(delay));
            }
        }

        
        //check which bunnies are vampire bunnies and add them to a counter
        int tochangeVamp = 0;
        for (Bunny&b : bunnies){
            if (b.Vamp){
                tochangeVamp++;
            }
        }
        
        
        int vampConversionMultiplier = (currentEvent == Eclipse ? 2 : 1);


        for (int i = 0; i < tochangeVamp*vampConversionMultiplier; i++){
            bool infected = false; 
            int safety = 1000;  
            while (!infected && safety-- > 0) {  
                int indx = rand() % bunnies.size();  
                if (!bunnies[indx].Vamp) {  
                    bunnies[indx].Vamp = true;  
                    // do NOT overwrite sex
                    if (rand() % 2 == 0) bunnies[indx].colour = "Black";  
                    else bunnies[indx].colour = "Red";  

                    infected = true;  
                    logLine("⚡ " + bunnies[indx].name + " was cursed!");  
                }  
            }
        }
        
        year++;
        logLine( "==== END OF YEAR " + to_string(year) + " ====");
        this_thread::sleep_for(chrono::milliseconds(delay));
        logLine( "Year summary: "
         + to_string(birthsThisYear) + " births, "
         + to_string(deathsThisYear) + " deaths, "
         + to_string(bunnies.size()) + " total bunnies.");
        
        this_thread::sleep_for(chrono::milliseconds(delay));
        switch(currentEvent){
            case None: break;
            case HarshWinter: logLine("Winter fades, the few survivors endure..."); break;
            case AbundantSpring: logLine("Spring bounty disappears as summer arrives."); break;
            case PredatorSeason: logLine("Predators retreat — peace returns for now."); break;
            case Eclipse: logLine("The eclipse fades, but something sinister lingers..."); break;
            case Plague: logLine("The Plague has now died off....but more danger is yet to come.");break;
        }
        this_thread::sleep_for(chrono::milliseconds(delay));
        logLine( "==== YEAR " + to_string(year+1) + " BEGINS ====");
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
        
        
        
};


int main(){
    srand(time(0));
    
    vector<string> maleNames = {
    "Thumper","Cocoa","Pebble","Patch","Dusty","Ash","Storm","Nimbus",
    "Nugget","Butterscotch","Echo","Frosty","Cinder","Oak","Biscuit","Snickers",
    "Mocha","Hazelnut","Button","Cashew","Pebbleton","Pip","Munch","Walnut",
    "Smudge","Tinsel","Pepper","Shadow","Flint","Copper","Hickory","Bolt",
    "Sprig","Comet","Orbit","Chip","Squish","Rascal","Dandelion","Chirp","Fig",
    "Bean","Rusty","Acorn","Bruno","Fable","Jasper","Drift","Chai"
    };
    vector<string> femaleNames = {
    "Luna","Daisy","Hazel","Clover","Maple","Poppy","Willow","Peach","Honey",
    "Muffin","Pumpkin","Freckles","Mochi","Cupcake","Nibble","Berry","Sprinkle",
    "Buttercup","Cinnamon","Twinkle","Tulip","Mochi","Juniper","Misty","Rosie",
    "Petal","Skye","Aurora","Fawn","S'more","Pearl","Blossom","Bluebell",
    "Sunshine","Cuddles","Pudding","Wisteria","Opal","Marshmallow","Sage",
    "Velvet","Coral","Minty","Dream","Whimsy","Sprout","Petunia","Lavender",
    "Galaxy"
    };
    vector<string> vampireNames = {
    "Nocturne","Raven","Shade","Hollow","Nyx","Obsidian","Vesper","Midnight",
    "Astra","Umbra","Salem","Moros","Crimson","Banshee","Fang","Spectre",
    "Requiem","Thorn","Hemlock","Sable","Eclipse","Ashen","Ruin","Onyx",
    "Grim","Wraith","Silence","Nightfall","Tempest","Shadowmire","Dread",
    "Phantom","Omen","Rook","Cinder","Grave","Blackthorn","Sorrow","Elder",
    "Shiver","Dire","Crypt","Vex","Bleak","Velour","Gloom","Rift","Widow",
    "Dustfall","Rooke"
    };
    vector<string>colourlist = {"Black", "Brown", "Spotted", "Striped", "Pink", "Purple", "Orange", "Red", "Silver", "Gold", "Bronze", "Yellow", "Green", "Blue"};
    //initialize farm class calling it "init"
    Farm init(maleNames, femaleNames, vampireNames, colourlist, 500);
    init.logLine( "Let's Begin The Bunny Simulation!" );
    while (!init.bunnies.empty()){
        init.printfarm();
        init.update();
       
        
    }
    init.logLine("All bunnies died. No bunnies left. :(");
    init.logfile.close();
    
    return 0;
}