

#include <iostream>
#include <fstream>
#include "Lot.h"
#include "json.hpp"
using json = nlohmann::json;




int main(void) {
	
	std::vector<Lot *> lot_list; 	//holds lots 

	
	std::ifstream myfile("Lots.txt"); //opens Lots.txt -- this should have the name of a lot on each line
	if (myfile.is_open()) 
	{
		std::string line;
		std::string cap;
		std::string name;
		while (getline(myfile, line))
		{
			name = line;
			getline(myfile, line);
			cap = line;
			getline(myfile, line);
			lot_list.push_back(new Lot(name,cap, line));  //creates Lots objects and puts them in the lot_list
		}
		myfile.close();
	}

	else std::cout << "Unable to open Lots";

	
	
	
		std::string lotInfo = "[";

		json j;	
//		std::string holder= "[";
		char chCheckForEscKey = 0;
		int size = lot_list.size();
		while (chCheckForEscKey != 27) { //while loops for the added cameras, will create a forloop to go through each of the feeds.
			//std::ofstream data;
			
			
			for (int i = 0; i < size; i++) {
				lot_list[i]->runFeeds();
			j[lot_list[i]->getName()] =  { "curCap", lot_list[i]->getCount() } ;	
			 //= { { "name", lot_list[i]->getName() },{ "curCap", lot_list[i]->getCount() }, {"maxCap", lot_list[i]->getCap() } };
				if(i == (size -1)){
 					lotInfo = lotInfo + "{ \"Name\":\"" + lot_list[i]->getName() + "\", \"Occupancy\":" + lot_list[i]->getCount() + ", \"Capacity\":" + lot_list[i]->getCap() + "}";
 				}	
				else{	
					lotInfo = lotInfo + "{ \"Name\":\"" + lot_list[i]->getName() + "\", \"Occupancy\":" + lot_list[i]->getCount() + ", \"Capacity\":" + lot_list[i]->getCap() + "},";  			
				}
 			}		
			lotInfo = lotInfo + "]";
			std::ofstream count;//file for count
			std::ofstream data; //file 
			data.open("lots.json", std::ios::trunc);
			data <<  lotInfo;
			count.open("count.json", std::ios::trunc);
                        count <<  j;

			
			lotInfo = "[";
			chCheckForEscKey = cv::waitKey(1);
			
			
			data.close();
			data.clear();
		}
		

		if (chCheckForEscKey != 27) {               // if the user did not press esc (i.e. we reached the end of the video) @CW will go until user hits Q, removed if video ends, because rtsp feed should never end
			cv::waitKey(0);                         // hold the windows open to allow the "end of video" message to show
		}
	

	
	
	
		return 0;
}

