#include<bits/stdc++.h>
#include<time.h>
#define MAX_CITIES 100
#define MAX_DIST 50
#define MAX_TOUR (MAX_CITIES * MAX_DIST)
#define MAX_ANTS 100
#define ALPHA 1.0
#define BETA 2.0 
#define RHO 0.1
#define QVAL 100
#define MAX_TOURS 15
#define MAX_TIME 20000
#define INIT_PHER 1.0

using namespace std;

struct cityType{
char c;
};
 
struct antType{
 
int curCity, nextCity, pathIndex;
int tabu[MAX_CITIES];
int path[MAX_CITIES];
double tourLength;
};



cityType cities[MAX_CITIES];
antType ants[MAX_ANTS];
 
double dist[MAX_CITIES][MAX_CITIES];
 
double phero[MAX_CITIES][MAX_CITIES];
 
double best=(double)MAX_TOUR;
int bestIndex;

void init()
{
    int from,to,ant;
 
    for(from = 0; from < MAX_CITIES; from++)
      cities[from].c='a'+from;
   cout<<"\nenter path matrix\n";   
      for(int i=0;i<MAX_CITIES;i++)
         for(int j=0;j<MAX_CITIES;j++)
             {   if(i==j)  dist[i][j]=0;  else { dist[i][j]=rand()%20; dist[j][i]=dist[i][j]; } }

  for(int i=0;i<MAX_CITIES;i++){
  
         for(int j=0;j<MAX_CITIES;j++)
          
			 {  if(dist[i][j]==0&&i!=j) dist[i][j]=INT_MAX;   cout<<dist[i][j]<<" "; } cout<<"\n"; }        
 
    to = 0;
    for( ant = 0; ant < MAX_ANTS; ant++)
    {
        if(to == MAX_CITIES)
            to=0;
 
        ants[ant].curCity = to++;
 
        for(from = 0; from < MAX_CITIES; from++)
        {
            ants[ant].tabu[from] = 0;
            ants[ant].path[from] = -1;
        }
 
        ants[ant].pathIndex = 1;
        ants[ant].path[0] = ants[ant].curCity;
        ants[ant].nextCity = -1;
        ants[ant].tourLength = 0;
 
        ants[ant].tabu[ants[ant].curCity] =1;
 
    }
  
    for(int i=0;i<MAX_CITIES;i++)
         for(int j=0;j<MAX_CITIES;j++)
          phero[i][j]=1;
}

void emitDataFile(int bestIndex)
{
    ofstream f1;
    f1.open("Data.txt");
    antType antBest;
    antBest = ants[bestIndex];  
    //f1<<antBest.curCity<<" "<<antBest.tourLength<<"\n";
    int i;
    for(i=0;i<MAX_CITIES;i++)
    {
        f1<<antBest.path[i]<<" ";
    }
 
    f1.close();
 
    f1.open("city_data.txt");
    for(i=0;i<MAX_CITIES;i++)
    {
        f1<<cities[i].c<<"\n";
    }
    f1.close();
 
}

void restartAnts()
{
    int ant,i,to=0;
 
    for(ant = 0; ant<MAX_ANTS; ant++)
    {
        if(ants[ant].tourLength <= best)
        {
            best = ants[ant].tourLength;
            bestIndex = ant;
            emitDataFile(bestIndex);
        }
 
        ants[ant].nextCity = -1;
        ants[ant].tourLength = 0.0;
 
        for(i=0;i<MAX_CITIES;i++)
        {
            ants[ant].tabu[i] = 0;
            ants[ant].path[i] = -1;
        }
 
        if(to == MAX_CITIES)
            to=0;
 
        ants[ant].curCity = to++;
 
        ants[ant].pathIndex = 1;
        ants[ant].path[0] = ants[ant].curCity;
 
        ants[ant].tabu[ants[ant].curCity] = 1;
    }
}

float antProduct(int from, int to)
{
    return(( pow( phero[from][to], ALPHA) * pow( (1.0/ dist[from][to]), BETA)));
}
 
int selectNextCity( int ant )
{
    int from, to;
    float denom = 0.0;
 
    from=ants[ant].curCity;
 
    for(to=0;to<MAX_CITIES;to++)     
	{       if(ants[ant].tabu[to] == 0)         
	{           denom += antProduct( from, to );        }   }         
	   
	  do {  float p;  to++;  if(to >= MAX_CITIES)  to=0;  
                if(ants[ant].tabu[to] == 0)
                {  
                  p = (antProduct(from,to)/denom);  
                  float x = (((float)rand()/RAND_MAX));  
                     if(x < p)   break; 
                }
          }while(1);
 
    return to;
}

int simulateAnts()
{
    int k;
    int moving = 0;
 
    for(k=0; k<MAX_ANTS; k++)
    {
 
        if( ants[k].pathIndex < MAX_CITIES )         { ants[k].nextCity = selectNextCity(k);     ants[k].tabu[ants[k].nextCity] = 1;             ants[k].path[ants[k].pathIndex++] = ants[k].nextCity;                       ants[k].tourLength += dist[ants[k].curCity][ants[k].nextCity];                      //handle last case->last city to first
 
            if(ants[k].pathIndex == MAX_CITIES)
            {
                ants[k].tourLength += dist[ants[k].path[MAX_CITIES -1]][ants[k].path[0]];
            }
 
            ants[k].curCity = ants[k].nextCity;
            moving++;
 
        }
    }
 
    return moving;
}
 
void updateTrails()
{
    int from,to,i,ant;
 
    for(from=0; from<MAX_CITIES;from++)
    {
        for(to=0;to<MAX_CITIES;to++)
        {
            if(from!=to)
            {
                phero[from][to] *=( 1.0 - RHO);
 
                if(phero[from][to]<0.0)
                {
                    phero[from][to] = INIT_PHER;
                }
            }
        }
    }
 
 
    for(ant=0;ant<MAX_ANTS;ant++)
    {
        for(i=0;i<MAX_CITIES;i++)
        {
            if( i < MAX_CITIES-1 )
            {
                from = ants[ant].path[i];
                to = ants[ant].path[i+1];
            }
            else
            {
                from = ants[ant].path[i];
                to = ants[ant].path[0];
            }
 
            phero[from][to] += (QVAL/ ants[ant].tourLength);
            phero[to][from] = phero[from][to];
 
        }
    }
 
  /*  for (from=0; from < MAX_CITIES;from++)
    {
        for( to=0; to<MAX_CITIES; to++)
        {
            phero[from][to] *= RHO;
        }
    } */
 
}

int main()
{
	
	srand((unsigned)time(NULL)%1000);
    int curTime = 0;
    cout<<"MaxTime="<<MAX_TIME;
 
 
    init();
 
    while( curTime++ < MAX_TIME)
    { 
        if( simulateAnts() == 0)
        { 
            updateTrails();
 
            if(curTime != MAX_TIME)
                restartAnts();
 
            cout<<"\nTime is "<<curTime<<"("<<best<<")";
 
        }
    }
 
    cout<<"\nBest tour = "<<best<<endl;
 
 
    return 0;
}
