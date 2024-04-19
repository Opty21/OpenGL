#pragma once

#include "global.h"
#include <iostream>
#include <fstream>
#include "CMesh.h"
#include "CTexture.h"
#include "Player.h"
#include "Ground.h"

#include<tuple> 
using namespace std;

// ::::::::::::::::::::::::::: Board ::::::::::::::::::::::::::
// :: Legend.................:      :.................Legend ::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// :: Wall...................: #  # :...................Wall ::
// :: Pusher.................: p  @ :.................Pusher ::
// :: Pusher on goal square..: P  + :..Pusher on goal square ::
// :: Box....................: b  $ :....................Box ::
// :: Box on goal square.....: B  * :.....Box on goal square ::
// :: Goal square............: .  . :............Goal square ::
// :: Floor..................:      :..................Floor ::
// :: Floor..................: -  _ :..................Floor ::
// ::                                                        ::


class LevelManager
{
    
public:
    int currentLevel=1;
    int longestLine=0;
    int height=0;
    const char *  filename = "";
    CMesh level,fences;
    vector<vector<char>> mapMatrix; 
    CGround GMesh;
    vector<Collidable> boxes;
    vector<CMesh> goals;
    string levelName = "";
    tuple <int,int> playerInitPos;
    CMesh goal;
    Collidable box;
    CTexture fenceTex,groundTex,boxTex;

    void init(int levelNo, const char *  file, CPlayer &player){
        currentLevel = levelNo;
        filename = file;
        longestLine=0;
        height=0;
        generateMapMatrix();
        //printMap();
        defineFloor();
        generateMesh(_tileScale);

        fences.CreateFromOBJ("fence.obj");
        
        fenceTex.CreateTexture("cer2.png");
        groundTex.CreateTexture("cer.png");
        level.attachTexture(groundTex);
        level.attachMaterial(glm::vec3(0.2 , 0.2 , 0.2),glm::vec3(1.0 , 1.0 , 1.0), glm::vec3(1.0 , 1.0 , 1.0), 32.0 );
        fences.attachTexture(fenceTex);
        GMesh.Init(level.OBJ_vertices);
        generateWalls();        
        player.setGround(GMesh);
        player.SetPosition(glm::vec3(get<1>(playerInitPos) * _tileScale + _tileScale/2.0f,0,get<0>(playerInitPos) * _tileScale + _tileScale/2.0f));
        player.Move(0);



        generateBoxesAndGoals();
        printMap();
        
        
    }
    glm::vec3 calculateCameraPositionForMap(){
        float x = longestLine/2.0f * _tileScale;
        float z = height/2.0f * _tileScale;

        float y = max(x,z) /( glm::tan(glm::radians(40.0f))); //tw sinusow
        return glm::vec3(-x,-y,-z);
    }

    glm::mat4 calculateScreenPos(){
        glm::mat4 tempMat = glm::rotate( glm::mat4(1.0), glm::radians(-90.f) , glm::vec3( 0.0f, 1.0f, 0.0f ) );
        tempMat = glm::translate(tempMat, glm::vec3(-2*_tileScale,15,-longestLine/2 * _tileScale));
        tempMat = glm::rotate( tempMat, glm::radians(-15.f) , glm::vec3( 0.0f, 0.0f, 1.0f ) );

        return tempMat;
    }

    void generateBoxesAndGoals(){
        box.matModelArray.clear();
        box.matModelArray.shrink_to_fit();
        goal.matModelArray.clear();
        goal.matModelArray.shrink_to_fit();
        box.CreateFromOBJ("box.obj");
        boxTex.CreateTexture("crate2.png");
        box.attachTexture(boxTex);
        for(int i=0;i<mapMatrix.size();i++){
            for(int j=0;j<mapMatrix[i].size();j++){
                if(mapMatrix[i][j] == '$' || mapMatrix[i][j] == '*'){
                    //SPAWN BOXES
                    cout<< "spawning a box" << endl;
                    box.addToMatrixArray(glm::vec3(j * _tileScale + _tileScale/2.0f,0,i * _tileScale + _tileScale/2.0f),glm::vec3(0,1,0),0,1);
                }
            }
        }
        box.setInstantiated();
        
        goal.CreateFromOBJ("donut.obj");
        goal.attachTexture(fenceTex);
        for(int i=0;i<mapMatrix.size();i++){
            for(int j=0;j<mapMatrix[i].size();j++){
                if(mapMatrix[i][j] == '.' || mapMatrix[i][j] == '*' ){
                    //SPAWN GOAL
                    cout<< "spawning goal" << endl;   
                    goal.addToMatrixArray(glm::vec3(j * _tileScale + _tileScale/2.0f,0,i * _tileScale + _tileScale/2.0f),glm::vec3(0,1,0),0,1);
                }
            }
        }
        goal.setInstantiated();
    }

    void generateWalls(){
        fences.matModelArray.clear();
        char symbolChars[4] = {'-', '$', '.','*'};
        for(int i=0;i<mapMatrix.size();i++){
            for(int j=0;j<mapMatrix[i].size();j++){
                if(mapMatrix[i][j] == '#'){
                    if(checkSpot(i-1,j,symbolChars,4)){
                    fences.addToMatrixArray(glm::vec3(j*_tileScale + _tileScale/2.0f,0,i*_tileScale),glm::vec3(0,1,0),0,_tileScale/2.0f);
                    }
                    if(checkSpot(i+1,j,symbolChars,4)){
                    fences.addToMatrixArray(glm::vec3(j*_tileScale + _tileScale/2.0f,0,i*_tileScale + _tileScale),glm::vec3(0,1,0),0,_tileScale/2.0f);
                    }
                    if(checkSpot(i,j-1,symbolChars,4)){
                    fences.addToMatrixArray(glm::vec3(j*_tileScale,0,i*_tileScale + _tileScale/2.0f),glm::vec3(0,1,0),90,_tileScale/2.0f);
                    }
                    if(checkSpot(i,j+1,symbolChars,4)){
                    fences.addToMatrixArray(glm::vec3(j*_tileScale + _tileScale,0,i*_tileScale + _tileScale/2.0f),glm::vec3(0,1,0),90,_tileScale/2.0f);
                    }
                }
            }
        }
        fences.setInstantiated();
    }

    bool checkSpot(int x,int y,char symbolArray[],int arraySize){
        if(x < 0 || y < 0) return false;
        if(x >= mapMatrix.size() || y >= mapMatrix[x].size()) return false;
        for(int j=0;j < arraySize;j++){
            if(mapMatrix[x][y] == symbolArray[j])return true;
        }
        return false;
    }

    void generateMesh(int _tileScale){
        level.createFromMatrix(mapMatrix,_tileScale);

    }
    
    bool makeMove(glm::vec3 position, glm::vec3 direction){
        char symbolChars[2] = {'-', '.'};

        glm::vec3 temp =position - glm::vec3(0.5f * _tileScale,0,0.5f * _tileScale);
        temp.x = temp.x / _tileScale;
        temp.z = temp.z / _tileScale;
        temp = glm::round(temp);
        cout << temp.x << " " << temp.z << endl;
        
        int checkX = temp.x + direction.x;
        int checkY = temp.z + direction.z;
        if(checkSpot(checkY,checkX,symbolChars,2)){
            string s;
            s += mapMatrix[temp.z][temp.x];
            s += mapMatrix[checkY][checkX];
            //cout << s << endl;
            if(s == "$-"){
                mapMatrix[temp.z][temp.x] = '-';
                mapMatrix[checkY][checkX] = '$';
            } else if(s == "$."){
                mapMatrix[temp.z][temp.x] = '-';
                mapMatrix[checkY][checkX] = '*';
            }else if(s == "*-"){
                mapMatrix[temp.z][temp.x] = '.';
                mapMatrix[checkY][checkX] = '$';
            }else if(s == "*."){
                mapMatrix[temp.z][temp.x] = '.';
                mapMatrix[checkY][checkX] = '*';
            }


            return true;
        } 

        return false;
    }

    bool checkForWin(){
        for(int i=0;i<mapMatrix.size();i++){
            for(int j=0;j<mapMatrix[i].size();j++){
                if(mapMatrix[i][j] == '.'){
                    return false;
                }
            }
        }
        return true;
    }

    void generateMapMatrix(){
        string temp = "";
        mapMatrix.clear();
        ifstream MyReadFile(filename);
        bool foundCorrect = false;
        while (getline (MyReadFile, temp)) {
            if(foundCorrect){
                if(temp[0] == 'T'){
                    levelName = temp;
                    break;
                } 
                vector<char> t;
                for(char const &character: temp){
                    char xd= character;
                    switch(xd){
                        case '$': case 'b': //box
                            xd = '$';
                            break;
                        case 'B': case '*': //box on goal;
                            xd = '*';
                            break;
                        case 'p': case '@': // player
                            xd = ' ';
                            playerInitPos = make_tuple(mapMatrix.size(),t.size());
                            break;
                        case 'P': case '+':
                            xd = '.';
                            playerInitPos = make_tuple(mapMatrix.size(),t.size());
                            break;
                        default:
                            break;
                    }
                    t.push_back(xd);
                }
                mapMatrix.push_back(t);
                if(temp.length() > longestLine) longestLine = temp.length();
                height+=1;
            }
            if(temp == to_string(currentLevel)) foundCorrect = true;
        }
        MyReadFile.close();
        std::cout << "Player Pos: " << get<0>(playerInitPos) << "x" << get<1>(playerInitPos) << std::endl;
        return;
    }

    void printMap(){
        for(auto const &j: mapMatrix){
            for(auto const &k: j){
                std::cout << k;

            } 
            std::cout << std::endl;
        } 
    }

    void defineFloor(){
        //DIJKSTRA 
        vector<tuple<int,int>> visited;
        vector<tuple<int,int>> toVisit;
        toVisit.push_back(playerInitPos);
        vector<tuple<int,int>>::iterator it;
        int counter = 0;
        while(toVisit.size() > 0){
            counter +=1;
            tuple<int,int> curr = toVisit.back();
            int currY = get<0>(curr);
            int currX = get<1>(curr);
            toVisit.pop_back();
            // std::find function call
            if(mapMatrix[currY][currX] == '#'){
                visited.push_back(curr);
                continue;
            }
            it = find(visited.begin(), visited.end(), curr);
            if (it != visited.end()) continue;

            visited.push_back(curr);
            toVisit.push_back(make_tuple(currY+1,currX));
            toVisit.push_back(make_tuple(currY-1,currX));
            toVisit.push_back(make_tuple(currY,currX+1));
            toVisit.push_back(make_tuple(currY,currX-1));
            if(mapMatrix[currY][currX] == ' ') mapMatrix[currY][currX] = '-';

        }
    };

    void setFile(const char *  file){
        filename = file;
    }

    void setLevel(int i){
        currentLevel = i;
    }

};
