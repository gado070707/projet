#ifndef CAPTEUR_H
#define CAPTEUR_H

class Capteur
{
    public:
        Capteur();
        Capteur(int);
		Capteur(int,int);
		Capteur(int,int,int);
		~Capteur();
		
		int getPin();
		int getPeriodeInitialisation();
		bool getReadEtat();
		bool getInitialisation();
		bool getInitialisationComplete();
		static bool getDeclenchement();
		
		void outHigh();
		void outLow();
		void input();
		void output();
		void bipEnContinue(unsigned long tempsEntreLesBips, unsigned long nombreDeBIP);
		bool periodeInitialisation();
		static int nombreInstances();
		
		static bool ms_declenchement;
		
        //virtual ~Capteur();

    protected:

    private:
        int m_pin;
		int m_periodeInitialisation;
		unsigned long m_millisDepart;
		unsigned long m_millisFin;
        bool m_etat;
		bool m_initialisation;
		bool m_initialisationComplete;
		bool m_initialisationPrintfs;
		
		static int ms_compteur;
};

#endif // CAPTEUR_H
