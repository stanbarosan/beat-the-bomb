#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


bool buton;

struct intrebare
{
    int nr;
    char text[100];
    bool raspuns;
};


int main()
{
    struct intrebare intrebari[15] = {{0,"Protecting by legislation of a minimum of 30% of the EU's land area and 30% of the EU's Sea",true},
                                      {1,"Protection of at least 10% of the EU’s protected areas, including primary and old-growth forests ",false},
                                      {2,"Adequate management of all protected areas, by defining clear conservation and monitoring objectives and measures",true},
                                      {3,"Protecting and restoring nature in the EU, by consolidating a coherent and effective network for protected areas and restoring degraded habitats",true},
                                      {4,"Enabling a new governance framework to ensure co-responsibility and co-ownership by allrelevant actors including setting up new financial opportunities",true},
                                      {5,"Adopting a global biodiversity agenda, to strengthen the contribution of the EU toward halting global biodiversity loss",true},
                                      {}};

}