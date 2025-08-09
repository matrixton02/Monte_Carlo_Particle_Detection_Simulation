#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <fstream>
#include <chrono>
using namespace std;

double dt=1e-12;

class Vector{                                                               // a vector class to represent positions and velocities
    public:
        double x;
        double y;
        double z;
        Vector(double a=0,double b=0,double c=0){                           //Constructor
            x=a;
            y=b;
            z=c;
        }
        void normalize(){                                                           
            double mag=sqrt(x*x+y*y+z*z);                                    //normlaizing the vector
            if(mag>0){
                x/=mag;
                y/=mag;
                z/=mag;
            }
        }
        void print(){
            cout<<"("<<x<<","<<y<<","<<z<<")";
        }

        bool operator==(const Vector& other){                               //overloading == operator
            if(x==other.x && y==other.y && z==other.z){
                return true;
            }
            return false;
        }
};

struct ParticleType{
    string type;
    double mass;
    int charge;
    double lifetime;
};

class Particle{
    public:
        string type;
        double mass;    //kg
        int charge;
        Vector pos;
        Vector vel;
        double energy;   //joules
        double lifetime; //second

        Particle(string type,double mass,int charge,Vector pos,Vector vel,double energy,double lifetime):type(type),mass(mass),charge(charge),pos(pos),vel(vel),energy(energy),lifetime(lifetime){}

        void move(double dt){
            pos.x+=vel.x*dt;
            pos.y+=vel.y*dt;
            pos.z+=vel.z*dt;
        }

        void scatter(double angle_stddev){                                 //determines a random scattering value to simulate real life collisions
            mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
            normal_distribution<> d(0, angle_stddev);
        
            double theta = d(gen);
            double speed = sqrt(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
        
            double new_x = vel.x * cos(theta) - vel.y * sin(theta);
            double new_y = vel.x * sin(theta) + vel.y * cos(theta);
        
            vel.x = new_x;
            vel.y = new_y;
            Vector dir(vel.x, vel.y, vel.z);
            dir.normalize();
            vel.x = dir.x * speed;
            vel.y = dir.y * speed;
            vel.z = dir.z * speed;
        }

        void lose_energy(double dE){
            energy-=dE;
            if(energy<0){
                energy=0;
            }
        }
        bool is_alive(double time_elapsed){
            return time_elapsed<lifetime;
        }

        void particle_stat(){
            cout<<type<<" particle at position ";
            pos.print();
            cout<<" with Energy="<<energy<<" J"<<endl;
        }
};

double angle_deviation(){                                           //to randomize the scattering of particles representing real life collisions
    mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<> angle_dis(0.0,2*M_PI);
    return angle_dis(gen);
}

Vector random_direction() {                                         //generating random direction to create the initial starting conditions of the simulation
    mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<> x_dis(0.5, 1.0);
    uniform_real_distribution<> y_dis(-0.1, 0.1);
    uniform_real_distribution<> z_dis(-0.1, 0.1);

    double x = x_dis(gen);
    double y = y_dis(gen);
    double z = z_dis(gen);
    Vector v(x, y, z);
    v.normalize();
    return v;
}

Vector random_position(double boxSize = 100) {                      //generating random position to create the initial starting conditions of the simulation
    mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<> disY(0.0, boxSize);
    uniform_real_distribution<> disZ(0.0, boxSize);
    uniform_real_distribution<> disX(40.0, 75.0);                   // particles start around center

    return Vector(disX(gen), disY(gen), disZ(gen));
}

double random_Energy(double minE=1e-14,double maxE=1e-12){          //All particles are initially given random energy
    mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<> dis(minE,maxE);

    return dis(gen);
}

Particle generate_random(){                                         // Genreates all the particles at timne t=0 for the simualtions
    vector<ParticleType> types={
        {"Electron",9.11e-31,-1,1e10},
        {"Muon",1.88e-28,-1,2.2e-6},
        {"Piom",2.49e-28,1,2.6e-8}
    };

    mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<> dis(0,2);
    ParticleType p=types[dis(gen)];

    Vector pos=random_position();
    Vector vel=random_direction();
    double energy=random_Energy();
    
    double speed = sqrt(2 * energy / p.mass);

    vel.normalize();
    vel.x *= speed;
    vel.y *= speed;
    vel.z *= speed;
    return Particle(p.type,p.mass,p.charge,pos,vel,energy,p.lifetime);
}

double random_dE(double energy){                                    //determines the radnom amount of energy that is reduced from the particle which signifies real life collsion
    mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<> dis(0.01*energy,0.05*energy);       //the particle may lose anywhere arounf 1% to 5% energy
    double dE=dis(gen);
    return dE;
}

bool random_hit(double probability){                                // it returns wether the probbaility that we detected if higher than a threshold
    mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<> dis(0.0,1.0);

    return dis(gen)<probability;
}

bool present(vector<pair<double,Particle>> decayed,Particle p){     //checks wether the particle is already in the decayed array or not
    int i=0;
    for(i=0;i<decayed.size();i++){
        Particle np=decayed[i].second;
        if(np.type==p.type && np.pos==p.pos && np.vel==p.vel && np.energy==p.energy){
            return true;
        }
    }
    return false;
}

void export_to_csv(const vector<pair<string,Particle>>& detected,const string& filename){                   //export all the data to a csv file
    ofstream file(filename);
    file<<"Detector,Type,Mass,Charge,Energy,PosX,PosY,PosZ,VelX,VelY,VelZ\n";

    for(const auto& entry: detected){
        const string& detector=entry.first;
        const Particle& p=entry.second;

        file<<detector<<","<<p.type<<","<<p.mass<<","<<p.charge<<","<<p.energy<<","<<p.pos.x<<","<<p.pos.y<<","<<p.pos.z<<","<<p.vel.x<<","<<p.vel.y<<","<<p.vel.z<<"\n";
    }
    file.close();
    cout<<"Data Successfully saved..."<<endl;
}

int main(){
    vector<pair<string,Particle>> detected;                         //stores detected data
    vector<pair<double,Particle>> decayed;                          //stores decayed particle
    vector<Particle> particle;
    double i=0;
    int j=0;
    for(i=0;i<20;i++){                                              //creating random particles for our simualtion
        Particle p=generate_random();
        particle.push_back(p);
    }

    for(i=0;i<1e-7;i+=dt){                                          //after every dt time we will run computations on all our particles
        for(j=0;j<particle.size();j++){
            Particle& p=particle[j];
            //p.particle_stat();                                    //used to print the particle state at each point to check for any inconsistency or error
            if(p.is_alive(i)){                                      // if the particle is alive then only we will perform the computations
                p.move(dt);                                         // we moved the particle
                p.lose_energy(random_dE(p.energy));                 //energy is deducted
                if((p.pos.x>=45 && p.pos.x<=50)){                     //checking wether it is in the detector range or not
                    if(random_hit(0.80)){
                        detected.push_back({"detector 1",p});
                    }
                }
                else if((p.pos.x>=55 && p.pos.x<=60)){
                    if(random_hit(0.85)){
                        detected.push_back({"detector 2",p});
                    }
                }
                else if((p.pos.x>=65 && p.pos.x<=70)){
                    if(random_hit(0.90)){
                        detected.push_back({"detector 3",p});
                    }
                }
                if(random_hit(0.01)){
                    p.scatter(angle_deviation());
                }
            }
            else{
                if(!present(decayed,p)){
                    decayed.push_back({i,p});
                }
            }
        }
    }
    export_to_csv(detected,"detected_particle.csv");
    return 0;
}