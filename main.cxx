#include "ParticleType.cxx"
#include "ResonanceType.cxx"
#include "Particle.cxx"
#include "TMath.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TRandom.h"

int main(){

gRandom->SetSeed();
constexpr int N = 120;
Particle::AddParticleType("P+",0.13957,+1);
Particle::AddParticleType("P-",0.13957,-1);
Particle::AddParticleType("K+",0.49367,+1);
Particle::AddParticleType("K-",0.49367,-1);
Particle::AddParticleType("p+",0.93827,+1);
Particle::AddParticleType("e-",0.93827,-1);
Particle::AddParticleType("K*",0.89166,0,0.050);
Particle particle[N];

TH1F *types = new TH1F("types","Types of Particles generated",7,0,7);
TH2F *correlation = new TH2F("correlation","distribution of angles",1000,0,TMath::Pi(),1000,0,2*TMath::Pi());
TH1F *impulse = new TH1F("impulse","Impulse distribution",1000,0,10);
TH1F *trasv_imp = new TH1F("trasv_imp","Traverse impulse distribution",1000,0,10);
TH1F *energy = new TH1F("energy","Energy of Particles distribution",1000,0,10);
TH1F *mass1 = new TH1F("mass1","Invariant Mass discord charge",1000,0,10);
TH1F *mass2 = new TH1F("mass1","Invariant Mass discord charge",1000,0,10);
TH1F *mass3 = new TH1F("mass1","Invariant Mass discord charge",1000,0,10);
TH1F *mass4 = new TH1F("mass1","Invariant Mass discord charge",1000,0,10);


for(int j=0; j<N; j++){//loop that fills mass8 THF1
    //if(particle[j].Get_indexParticle() == -2){ ++j;}
    
    if
}

}
