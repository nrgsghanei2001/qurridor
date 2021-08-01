#include <iostream>
#include "httplib.h"
#include "client.h"
#include "board.h"
using namespace std;
using namespace httplib;

clint player;



int main()
{
	Client cli("localhost", 8080);
	cout << "Please Enter your name : " ;
	cin >> player.name;
    
    /*
    struct MultipartFormData {
        std::string name;
        std::string content;
        std::string filename;
        std::string content_type;
    };
    */
	MultipartFormDataItems data = { {"Login", player.name,"",""} };

    if(auto res = cli.Post("/login", data))
    {
		if((res -> body) == "Invalid Request")
        {
			cout << "Login faild ! " << endl;
			return 0;
		}
		else
        {
			cout <<"You joined the game successfully !"<< endl;
		}
	}
	else
    {
		cout << "Something went wrong ! please check the connection to server ." << endl;
	}

    
	bool game_is_runing= true ;
	string choice="",movement="";
	while(game_is_runing)
	{
		string trn = "" ;
		trn += (char)(player.key + 48);
		string bord="";
		data = { {"Turn",trn,"",""} };
	
		if(auto res = cli.Post("/turn", data))
		{
			bord = res -> body;
			if(bord=="game is finished ")
			{
				cout<<"game is over !"<<endl;
				game_is_runing=false;
				return 0;
			}
		}	
        
		cout << "It's your turn "<<player.name<<endl<<"Please choose : " << endl;
		cout <<bord<<endl;
		cout << "Do you want to move(m) or put a wall(w) ? " << endl;
		cin >> choice ;

		if(choice == "m")
		{
			cout << "Do you want to move up(u) or down(d) or right(r) or left(l) ? :" << endl;
			cin >> movement ;
			data[0].name = "Move";
			data[0].content_type = movement;
			auto res = cli.Post("/move", data);
			bord=res -> body ;
			cout<<bord;
		}
		//auto res = cli.Post("/move", data);
		else if(choice == "w")
		{
			string w1,w2;
			cout << "Enter the places you want to put a wall  , exampla : 111 110 : " << endl;
			cin >> w1  >> w2 ;
			data[0].name = "Wall";
			data[0].filename = w1;
			data[0].content_type = w2;
			auto res = cli.Post("/wall", data);
			bord=res -> body ;
			cout<<bord;
		}

	}
}