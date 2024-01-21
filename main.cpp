#include <QtCore>
#include <QtGui>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QLineEdit>
#include <QMessageBox>
#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QPixmap>
#include <random>
#include <sstream>
#include <QProgressBar>
#include <QStyle>
#include <QToolTip>
#include <QEvent>
#include <QString>



    struct Mobs;
    struct Helden;

struct Helden
    {
        std::string Name;
        std::string Klasse;
        std::string Rasse;
        int hp;
        int maxhp;
        int exp;
        int lvl;
        int attackdmg;
        int magicdmg;
       
    
    public:
    Helden( const std::string& Name,
            const std::string& Klasse, 
            const std::string& Rasse, 
            int hp,
            int maxhp,
            int exp, 
            int lvl,
            int attackdmg, 
            int magicdmg) :
            Name(Name),
            Klasse(Klasse),
            Rasse(Rasse),
            hp(hp),
            maxhp(maxhp),
            exp(exp),
            lvl(lvl),
            attackdmg(attackdmg),
            magicdmg(magicdmg)
            {

            }
            void Attack(Mobs& mob);
            void MagicAttack(Mobs& mob);            

        std::string getInfo() const {
            std::ostringstream info;
            info << "Name: " << Name << "\n"
                 << "Klasse: " << Klasse << "\n"
                 << "Rasse: " << Rasse << "\n"
                 << "HP: " << hp << "/" << maxhp << "\n"
                 << "Exp: " << exp << "\n"
                 << "Level: " << lvl << "\n"
                 << "Attack Damage: " << attackdmg << "\n"
                 << "Magic Damage: " << magicdmg;
            return info.str();
        }
        void healthUpdated(const QString& message);
        void died(){
        //    MainWindow::switchToDeathscreen();
        }    
        void getDamage(int damage){
                hp = hp-damage;
                if(hp <= 0){
                    died();
                }
        }


        void lvlup(){
            lvl += 1;
            attackdmg = attackdmg + 10;
            magicdmg = magicdmg + 5;
            maxhp = maxhp + 10;
            hp += 10;
            exp = exp - 2000;
        }

        void getEXP(int xp){
            exp = exp + xp;
            if (exp >= 2000)
            {
                lvlup();
            }
        }
            
        void getHealth(int potion){//Einsatz eines Heiltrankes
            if(hp + potion <= maxhp)
                {
                    hp += potion;
                }else if (hp + potion > maxhp & hp < maxhp){                  
                    hp = maxhp;
                }
        }        

        };



    
    struct Mobs
    {
        std::string Name;
        std::string Rasse;
        int hp;
        int maxhp;
        int lvl;
        int attackdmg;
        int magicdmg;
    
    public:
    Mobs( const std::string& Name, 
            const std::string& Rasse, 
            int hp,
            int maxhp, 
            int lvl,
            int attackdmg, 
            int magicdmg) :
            Name(Name),
            Rasse(Rasse),
            hp(hp),
            maxhp(maxhp),
            lvl(lvl),
            attackdmg(attackdmg),
            magicdmg(magicdmg){}

        void Attack(Helden& held);

        std::string getInfo() const {
        std::ostringstream info;
            info << "Name: " << Name << "\n"
                 << "Rasse: " << Rasse << "\n"
                 << "HP: " << hp << "/" << maxhp << "\n"
                 << "Level: " << lvl << "\n"
                 << "Attack Damage: " << attackdmg << "\n"
                 << "Magic Damage: " << magicdmg;
            return info.str();
        }
            void getDamage(int damage){
                hp = hp - damage;             
            }


        
    };
            
        void Helden::Attack(Mobs& mob){
            mob.getDamage(attackdmg);
        }

        void Helden::MagicAttack(Mobs& mob){
            mob.getDamage(magicdmg);
        }       

        void Mobs::Attack(Helden& held){
            if(attackdmg > magicdmg){
            held.getDamage(attackdmg);
            }else{
                held.getDamage(magicdmg);
            }
        }

class MainWindow : public QWidget{
      CS_OBJECT(MainWindow)

public:
      MainWindow(QWidget *parent = nullptr) : QWidget(parent){
            setFixedSize(500, 500);
            initUI();
            loadEnemyData();
            loadCharData();
            createPages();
            setupConnections();


      }


private:
QStringList allyklassen;
QStringList allyrassen;
QStringList names;
QStringList rassen;
std::vector<Helden> Held{};
std::vector<Mobs> Mob{};
QLabel *rightCharInfoLabel;
QLabel *leftCharInfoLabel;
QLabel *leftCharLabel;
QProgressBar *playerHealthBar;
QProgressBar *enemyHealthBar;

int numbpotion_s = 3;
int numbpotion_m = 2;
int numbpotion_l = 4;

//LoadDataPart..........................................................
      void loadCharData(){
            loadFromFile("../Textdateien/allyKlassen", allyklassen);
            loadFromFile("../Textdateien/allyRassen", allyrassen);
      }
      void loadEnemyData() {
            loadFromFile("../Textdateien/Enemynames", names);
            loadFromFile("../Textdateien/EnemyRassen", rassen);
    }
      void loadFromFile(const QString& fileName, QStringList& container) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                container.append(line);
            }
            file.close();
        }
    }
//.........................................................................   
    




      void createEnemy() {
        // Code for creating an enemy with a random name, race, and class
        if (Mob.empty()) {
        int enemyattack;
        int enemymagic;
        int enemyhealth;
        int enemylvl;
        std::random_device rd; // Erzeugt ein zufälliges seed
        std::mt19937 gen(rd()); // Mersenne Twister Zufallsgenerator mit dem seed von random_device

        // Definition des Verteilungsbereichs
        std::uniform_int_distribution<> disrass(0, 5); // Zahlen zwischen 0 und 5
        std::uniform_int_distribution<> disname(0,19);
        std::uniform_int_distribution<> disclass(1,2);
        // Generieren einer Zufallszahl
        int ran_numb_name = disname(gen);
        int ran_numb_rass = disrass(gen);
        int ran_numb_class = disclass(gen);
        std::string enemyname = names[ran_numb_name].toStdString();
        std::string enemyrass = rassen[ran_numb_rass].toStdString();

        if(ran_numb_class == 1){
            enemyattack = Held[0].lvl*3 +5;
            enemymagic = Held[0].lvl + 3;
        }
        if(ran_numb_class == 2){
            enemyattack = Held[0].lvl+3;
            enemymagic = Held[0].lvl*3 + 5;            
        }
        enemylvl = Held[0].lvl + 1;
        enemyhealth = enemylvl*15 + 100;        

        Mobs newMob(enemyname, enemyrass, enemyhealth, enemyhealth, enemylvl, enemyattack, enemymagic);
        Mob.push_back(newMob);        
        
        updateUI();
        // Display the enemy information in the GUI or use it as needed
        //QMessageBox::information(this, "Enemy Created", QString::fromStdString(newMob.Name + "\n" + newMob.Rasse));
        }
        
       }


      void createCharacter(){
            int HP = 100;
            int AttackDamage = 15;
            int MagicDamage = 5;

        QDialog dialog(this);
        dialog.setWindowTitle("Create Character");

        QFormLayout formLayout(&dialog);

        QLineEdit nameLineEdit(&dialog);
        QComboBox classComboBox(&dialog);
        QComboBox raceComboBox(&dialog);
       
        // Populate class and race combo boxes
        classComboBox.addItems(allyklassen);
        raceComboBox.addItems(allyrassen);
        formLayout.addRow("Name:", &nameLineEdit);
        formLayout.addRow("Klasse:", &classComboBox);
        formLayout.addRow("Rasse:", &raceComboBox);

        QPushButton createButton("Create", &dialog);
        connect(&createButton, &QPushButton::clicked, &dialog, &QDialog::accept);
        
        formLayout.addRow(&createButton);

        if (dialog.exec() == QDialog::Accepted) {
            // Retrieve values from the dialog
            QString name = nameLineEdit.text();
            QString klasse = classComboBox.currentText();
            QString rasse = raceComboBox.currentText();

            // Validate and create new character
            if (allyklassen.contains(klasse) && allyrassen.contains(rasse)) {
                Helden newHeld(name.toStdString(), klasse.toStdString(), rasse.toStdString(), HP, HP, 0, 1, AttackDamage, MagicDamage);
                Held.push_back(newHeld);

                updateUI();
                // Display character information (you can replace this with your own logic)
                QMessageBox::information(this, "Character Created", QString::fromStdString("Character " + newHeld.Name + " created!"));
                switchToMM();
            } else {
                QMessageBox::warning(this, "Invalid Selection", "Please choose a valid class and race.");
            }
        }
      }

    void physicalAttack(){
        Held[0].Attack(Mob[0]);
        if(Mob[0].hp <= 0){
            Mob.pop_back();
            Held[0].getEXP(1000);
            switchToVictoryscreen();
        }else{
            Mob[0].Attack(Held[0]);
            if(Held[0].hp <= 0){
                Held.erase(Held.begin());
                Mob.pop_back();
                switchToDeathscreen();
            }
        }
        updateUI();
    }

    void Heal(int potion){
        int potion_s = 15;
        int potion_m = 30;
        int potion_l = 60;
        if(potion == 1){
            if(numbpotion_s > 0 ){
                int temphealth = Held[0].hp;
                Held[0].getHealth(potion_s);
                if(temphealth + potion_s <= Held[0].maxhp){
                    QMessageBox::information(this,"Heal", QString::fromStdString("Die HP von " + Held[0].Name + " wurde um " + std::to_string(potion_s) + " geheilt"));
                }else if(temphealth + potion_s > Held[0].maxhp & temphealth < Held[0].maxhp){
                    QMessageBox::information(this,"Information", QString::fromStdString("Die HP von " + Held[0].Name + " wurde um " + std::to_string(Held[0].maxhp - temphealth) + " geheilt"));
                }else if(temphealth = Held[0].maxhp){
                    QMessageBox::information(this,"Information", QString::fromStdString("Die HP von " + Held[0].Name + " sind schon voll"));
                }

                if(temphealth < Held[0].hp){
                    numbpotion_s = numbpotion_s - 1;
                }

            }else{
                QMessageBox::information(this,"Heal", "Du hast keine Tränke (S)");
            }
        }else if(potion == 2){
            if(numbpotion_m > 0 ){
                int temphealth = Held[0].hp;
                Held[0].getHealth(potion_m);
                if(temphealth + potion_m <= Held[0].maxhp){
                    QMessageBox::information(this,"Heal", QString::fromStdString("Die HP von " + Held[0].Name + " wurde um " + std::to_string(potion_m) + " geheilt"));
                }else if(temphealth + potion_m > Held[0].maxhp & temphealth < Held[0].maxhp){
                    QMessageBox::information(this,"Information", QString::fromStdString("Die HP von " + Held[0].Name + " wurde um " + std::to_string(Held[0].maxhp - temphealth) + " geheilt"));
                }else if(temphealth = Held[0].maxhp){
                    QMessageBox::information(this,"Information", QString::fromStdString("Die HP von " + Held[0].Name + " sind schon voll"));
                }

                if(temphealth < Held[0].hp){
                    numbpotion_m = numbpotion_m - 1;
                }
            }else{
                QMessageBox::information(this,"Heal", "Du hast keine Tränke (M)");
            }
        }else if(potion == 3){
            if(numbpotion_l > 0 ){
                int temphealth = Held[0].hp;
                Held[0].getHealth(potion_l);
                if(temphealth + potion_l <= Held[0].maxhp){
                    QMessageBox::information(this,"Heal", QString::fromStdString("Die HP von " + Held[0].Name + " wurde um " + std::to_string(potion_l) + " geheilt"));
                    
                }else if(temphealth + potion_l > Held[0].maxhp & temphealth < Held[0].maxhp){
                    QMessageBox::information(this,"Information", QString::fromStdString("Die HP von " + Held[0].Name + " wurde um " + std::to_string(Held[0].maxhp - temphealth) + " geheilt"));
                    
                }else if(temphealth == Held[0].maxhp){
                    QMessageBox::information(this,"Information", QString::fromStdString("Die HP von " + Held[0].Name + " sind schon voll"));
                    
                }

                if(temphealth < Held[0].hp){
                    numbpotion_l = numbpotion_l - 1;
                }
            }else{
                QMessageBox::information(this,"Heal", "Du hast keine Tränke (L)");
            }
        }
        updateLabels();
    }


     void updateHealthToolTip() {
        if (sender() == playerHealthBar) {
            QString playerTooltip = "Health: " + QString::number(playerHealthBar->value()) + " / " + QString::number(playerHealthBar->maximum());
            playerHealthBar->setToolTip(playerTooltip);
        } else if (sender() == enemyHealthBar) {
            QString enemyTooltip = "Health: " + QString::number(enemyHealthBar->value()) + " / " + QString::number(enemyHealthBar->maximum());
            enemyHealthBar->setToolTip(enemyTooltip);
        }
    }


    //SwitchScreenFunktions.............................................

      void switchToMM(){
            stackedWidget.setCurrentIndex(1);
      }

      void switchToKampfMenü(){
            stackedWidget.setCurrentIndex(2);
      }
      void switchToConsum(){
            stackedWidget.setCurrentIndex(3);
      }
      void switchToChar(){
            stackedWidget.setCurrentIndex(4);
      }
      void switchToShop(){
            stackedWidget.setCurrentIndex(5);
      }
      void switchToDeathscreen(){
            stackedWidget.setCurrentIndex(6);
      }
      void switchToVictoryscreen(){
            stackedWidget.setCurrentIndex(7);
      }
      void switchToHealScreen(){
            stackedWidget.setCurrentIndex(8);
      }

    CS_SIGNAL_1(Public, void characterCreated())
    CS_SIGNAL_2(characterCreated)
      QStackedWidget stackedWidget;    
//.............................................................................
      void initUI(){
            QVBoxLayout *mainLayout = new QVBoxLayout(this);
            mainLayout->addWidget(&stackedWidget);
      }

      void updateUI(){
        updateLabels();
      }

      void updateLabels() {
    if (!Held.empty()) {
        leftCharInfoLabel->setText(QString::fromStdString(Held[0].getInfo()));
        leftCharLabel->setText(QString::fromStdString(Held[0].getInfo()));
    } else {
        leftCharInfoLabel->setText("No character information available");
    }

    if (!Mob.empty()) {
        rightCharInfoLabel->setText(QString::fromStdString(Mob[0].getInfo()));
    } else {
        rightCharInfoLabel->setText("No Enemy information available");
    }

    if (!Held.empty()) {
        playerHealthBar->setRange(0, Held[0].maxhp);
        playerHealthBar->setValue(Held[0].hp);
    }

    if (!Mob.empty()) {
        enemyHealthBar->setRange(0, Mob[0].maxhp);
        enemyHealthBar->setValue(Mob[0].hp);
}
      }
      void createPages(){
            //StartScreen
            QWidget *StartMenü = new QWidget;
            QVBoxLayout *Startlayout = new QVBoxLayout(StartMenü);
            QPushButton *StartButton = new QPushButton("Game Start"); 
            StartButton->setObjectName("GameStartButton");  // Set the object name
            Startlayout->addWidget(StartButton);
            stackedWidget.addWidget(StartMenü);

            //CreateCharacter
            //QWidget *createCharacter = new QWidget;


            //MainMenü
            QWidget *MM = new QWidget;
            QVBoxLayout *MMlayout = new QVBoxLayout(MM);
            QPushButton *KampfButton = new QPushButton("Kämpfen");
            QPushButton *ConsumButton = new QPushButton("Verbrauchsgegenstände");
            QPushButton *CharButton = new QPushButton("Character");
            QPushButton *ShopButton = new QPushButton("Shop");
            QPushButton *BeendenButton = new QPushButton("Beenden");
            BeendenButton->setObjectName("BeendenButton");
            KampfButton->setObjectName("KampfButton");
            ConsumButton->setObjectName("ConsumButton");
            CharButton->setObjectName("CharButton");
            ShopButton->setObjectName("ShopButton");
            MMlayout->addWidget(KampfButton);
            MMlayout->addWidget(ConsumButton);
            MMlayout->addWidget(CharButton);
            MMlayout->addWidget(ShopButton);
            MMlayout->addWidget(BeendenButton);
            stackedWidget.addWidget(MM);

            //Kampfmenü
            QWidget *Kampfmenü = new QWidget;
            QVBoxLayout *KampfmenülayoutV = new QVBoxLayout(Kampfmenü);
            QHBoxLayout *KampfmenülayoutH = new QHBoxLayout(Kampfmenü);
            QLabel *leftlabel = new QLabel(QString("Kampfmenü"),Kampfmenü);
            leftCharInfoLabel = new QLabel;

            QPixmap leftoriginalPixmap("../Bilder/Swordsman.png");
            QPixmap leftscaledPixmap = leftoriginalPixmap.scaled(200, 200, Qt::KeepAspectRatio);
            leftlabel->setPixmap(leftscaledPixmap);

            if (!Held.empty()) {
                leftCharInfoLabel->setText(QString::fromStdString(Held[0].getInfo()));
            } else {
            // Handle the case when Held vector is empty
                leftCharInfoLabel->setText("No character information available");
            }


            QLabel *rightlabel = new QLabel(QString("Kampfmenü"),Kampfmenü);
            rightCharInfoLabel = new QLabel;

            QPixmap rightoriginalPixmap("../Bilder/Goblin.png");
            QPixmap rightscaledPixmap = rightoriginalPixmap.scaled(200, 200, Qt::KeepAspectRatio);
            rightlabel->setPixmap(rightscaledPixmap);

            if (!Mob.empty()) {
                rightCharInfoLabel->setText(QString::fromStdString(Mob[0].getInfo()));
            } else {
            // Handle the case when Held vector is empty
                rightCharInfoLabel->setText("No Enemy information available");
            }
            QPushButton *AttackButton = new QPushButton("Phyysischer Angriff");
            QPushButton *MagicButton = new QPushButton("Magischer Angriff");
            QPushButton *ZurückButton = new QPushButton("Zurück");

            AttackButton->setObjectName("AttackButton");
            MagicButton->setObjectName("MagicButton");
            ZurückButton->setObjectName("ZurückButton");


    playerHealthBar = new QProgressBar(Kampfmenü);
    enemyHealthBar = new QProgressBar(Kampfmenü);
    playerHealthBar->setOrientation(Qt::Horizontal);
    enemyHealthBar->setOrientation(Qt::Horizontal);
    if (!Held.empty()) {
        playerHealthBar->setRange(0, Held[0].maxhp);
        playerHealthBar->setValue(Held[0].hp);
    }

    if (!Mob.empty()) {
        enemyHealthBar->setRange(0, Mob[0].maxhp);
        enemyHealthBar->setValue(Mob[0].hp);
    }

        // Set stylesheet for health bars
    playerHealthBar->setStyleSheet("QProgressBar {"
                                   "border: 2px solid grey;"
                                   "border-radius: 5px;"
                                   "background: white;"
                                   "padding: 1px;"
                                   "text-align: center;"
                                   "}"
                                   "QProgressBar::chunk {"
                                   "background-color: #05B8CC;"
                                   "width: 20px;"
                                   "}");

    enemyHealthBar->setStyleSheet("QProgressBar {"
                                  "border: 2px solid grey;"
                                  "border-radius: 5px;"
                                  "background: white;"
                                  "padding: 1px;"
                                  "text-align: center;"
                                  "}"
                                  "QProgressBar::chunk {"
                                  "background-color: #FF6347;"
                                  "width: 20px;"
                                  "}");

    KampfmenülayoutV->addWidget(playerHealthBar);
    KampfmenülayoutV->addWidget(enemyHealthBar);

QHBoxLayout *infoLabelsLayout = new QHBoxLayout;
infoLabelsLayout->addWidget(leftCharInfoLabel);
infoLabelsLayout->addStretch(); // Add stretch to push the right label to the right
infoLabelsLayout->addWidget(rightCharInfoLabel);
KampfmenülayoutV->addLayout(infoLabelsLayout);


QHBoxLayout *ImageLayout = new QHBoxLayout;
ImageLayout->addWidget(leftlabel);
ImageLayout->addStretch();
ImageLayout->addWidget(rightlabel);
KampfmenülayoutV->addLayout(ImageLayout);

            KampfmenülayoutV->addWidget(AttackButton);
            KampfmenülayoutV->addWidget(MagicButton);
            KampfmenülayoutV->addWidget(ZurückButton);
            stackedWidget.addWidget(Kampfmenü);

            //ConsumMenü
            QWidget *ConsumMenü = new QWidget;
            QVBoxLayout *Consumlayout = new QVBoxLayout(ConsumMenü);
            QPushButton *HealthPotions = new QPushButton("Healthpotions");
            QPushButton *ZurückButton2 = new QPushButton("Zurück");
            HealthPotions->setObjectName("Potions");
            ZurückButton2->setObjectName("ZurückButton");
            Consumlayout->addWidget(HealthPotions);
            Consumlayout->addWidget(ZurückButton2); 
            stackedWidget.addWidget(ConsumMenü);                     
            
            //charMenü
            QWidget *CharMenü = new QWidget;
            QVBoxLayout *Charlayout = new QVBoxLayout(CharMenü);
            QLabel *leftlabelchar = new QLabel(QString("charMenü"),CharMenü);
            leftCharLabel = new QLabel;
            if (!Held.empty()) {
                leftCharLabel->setText(QString::fromStdString(Held[0].getInfo()));
            } else {
            // Handle the case when Held vector is empty
                leftCharLabel->setText("No character information available");
            }

            QPixmap leftoriginalPixmapchar("../Bilder/Swordsman.png");
            QPixmap leftscaledPixmapchar = leftoriginalPixmapchar.scaled(200, 200, Qt::KeepAspectRatio);
            leftlabelchar->setPixmap(leftscaledPixmapchar);
            QPushButton *ZurückButton3 = new QPushButton("Zurück");
            Charlayout->addWidget(leftCharLabel);
            Charlayout->addWidget(leftlabelchar);
            ZurückButton3->setObjectName("ZurückButton");
            Charlayout->addWidget(ZurückButton3); 
            stackedWidget.addWidget(CharMenü);

            //ShopMenü
            QWidget *ShopMenü = new QWidget;
            QVBoxLayout *Shoplayout = new QVBoxLayout(ShopMenü);
            QPushButton *ZurückButton4 = new QPushButton("Zurück");
            QPushButton *KaufButton = new QPushButton("Kaufen");
            QPushButton *VerkaufButton = new QPushButton("Verkaufen");
            KaufButton->setObjectName("Kaufen");
            VerkaufButton->setObjectName("Verkaufen");
            ZurückButton4->setObjectName("ZurückButton");
            Shoplayout->addWidget(KaufButton);
            Shoplayout->addWidget(VerkaufButton);
            Shoplayout->addWidget(ZurückButton4); 
            stackedWidget.addWidget(ShopMenü);                  

            //Deathscreen
            //QString tempName = QString::fromStdString(Held[0].Name);
            //Held.erase(Held.begin());
            //Mob.pop_back();
            QWidget *Deathscreen = new QWidget;
            QVBoxLayout *Deathlayout = new QVBoxLayout(Deathscreen);
            QLabel *deathLabel = new QLabel("You have died!", this);
            Deathlayout->addWidget(deathLabel);
            QPushButton *restartButton = new QPushButton("Beenden", this);
            restartButton->setObjectName("DeathEnd");
            Deathlayout->addWidget(restartButton);
            stackedWidget.addWidget(Deathscreen);

            //VictoryScreen
            QWidget *VictoryScreen = new QWidget;
            QVBoxLayout *Victorylayout = new QVBoxLayout(VictoryScreen);
            QLabel *victoryLabel = new QLabel("You won", this);
            Victorylayout->addWidget(victoryLabel);
            QPushButton *VictoryButton = new QPushButton("Zurück zum Hauptmenü");
            VictoryButton->setObjectName("VictoryButton");
            Victorylayout->addWidget(VictoryButton);
            stackedWidget.addWidget(VictoryScreen);

            //HealScreen
            QWidget *HealScreen = new QWidget;
            QVBoxLayout *Heallayout = new QVBoxLayout(HealScreen);
            QPushButton *ButtonPotion_s = new QPushButton("Use Potion (S)");
            QPushButton *ButtonPotion_m = new QPushButton("Use Potion (M)");
            QPushButton *ButtonPotion_l = new QPushButton("Use Potion (L)");
            QPushButton *ZurückButton5 = new QPushButton("Zurück");
            ButtonPotion_s->setObjectName("Potion_s");
            ButtonPotion_m->setObjectName("Potion_m");
            ButtonPotion_l->setObjectName("Potion_l");
            ZurückButton5->setObjectName("Zurück");
            Heallayout->addWidget(ButtonPotion_s);
            Heallayout->addWidget(ButtonPotion_m);
            Heallayout->addWidget(ButtonPotion_l);
            Heallayout->addWidget(ZurückButton5);
            stackedWidget.addWidget(HealScreen);

            stackedWidget.setCurrentIndex(0);




      }
      void setupConnections() {

        connect(stackedWidget.widget(0)->findChild<QPushButton*>("GameStartButton"), &QPushButton::clicked, this, &MainWindow::createCharacter);
        connect(stackedWidget.widget(1)->findChild<QPushButton*>("BeendenButton"), &QPushButton::clicked, this, &QWidget::close);
        connect(stackedWidget.widget(1)->findChild<QPushButton*>("KampfButton"),&QPushButton::clicked, this, &MainWindow::createEnemy);
        connect(stackedWidget.widget(1)->findChild<QPushButton*>("KampfButton"),&QPushButton::clicked, this, &MainWindow::switchToKampfMenü);
        connect(stackedWidget.widget(1)->findChild<QPushButton*>("ConsumButton"),&QPushButton::clicked, this, &MainWindow::switchToConsum);
        connect(stackedWidget.widget(1)->findChild<QPushButton*>("CharButton"),&QPushButton::clicked, this, &MainWindow::switchToChar);
        connect(stackedWidget.widget(1)->findChild<QPushButton*>("ShopButton"),&QPushButton::clicked, this, &MainWindow::switchToShop);
        connect(stackedWidget.widget(2)->findChild<QPushButton*>("ZurückButton"),&QPushButton::clicked, this, &MainWindow::switchToMM);
        connect(stackedWidget.widget(2)->findChild<QPushButton*>("AttackButton"),&QPushButton::clicked, this, &MainWindow::physicalAttack);
        connect(stackedWidget.widget(3)->findChild<QPushButton*>("ZurückButton"),&QPushButton::clicked, this, &MainWindow::switchToMM);
        connect(stackedWidget.widget(3)->findChild<QPushButton*>("Potions"),&QPushButton::clicked, this, &MainWindow::switchToHealScreen);
        connect(stackedWidget.widget(4)->findChild<QPushButton*>("ZurückButton"),&QPushButton::clicked, this, &MainWindow::switchToMM);
        connect(stackedWidget.widget(5)->findChild<QPushButton*>("ZurückButton"),&QPushButton::clicked, this, &MainWindow::switchToMM);
        connect(stackedWidget.widget(6)->findChild<QPushButton*>("DeathEnd"),&QPushButton::clicked, this, &QWidget::close);
        connect(stackedWidget.widget(7)->findChild<QPushButton*>("VictoryButton"),&QPushButton::clicked, this, &MainWindow::switchToMM);
        connect(stackedWidget.widget(8)->findChild<QPushButton*>("Potion_s"), &QPushButton::clicked, this, [this]() { Heal(1); });
        connect(stackedWidget.widget(8)->findChild<QPushButton*>("Potion_m"), &QPushButton::clicked, this, [this]() { Heal(2); });
        connect(stackedWidget.widget(8)->findChild<QPushButton*>("Potion_l"), &QPushButton::clicked, this, [this]() { Heal(3); });
        connect(stackedWidget.widget(8)->findChild<QPushButton*>("Zurück"),&QPushButton::clicked, this, &MainWindow::switchToConsum);     
        connect(this, &MainWindow::characterCreated, this, &MainWindow::updateUI);

        connect(playerHealthBar, &QProgressBar::valueChanged, this, &MainWindow::updateHealthToolTip);
    connect(enemyHealthBar, &QProgressBar::valueChanged, this, &MainWindow::updateHealthToolTip);

    }


};

int main(int argc, char *argv[])
{
      QApplication app(argc, argv);

      MainWindow mainWindow;
      mainWindow.show(); 
   return app.exec();
}