#include "Lot.h"


//#include<opencv2/core/core.hpp>


Lot::Lot(cv::String lotname, cv::String cap, cv::String lotid) {
	
	Lot_name = lotname;
	id = lotid;
	
	capacity = cap;
		std::string feed_name;
		std::string line;
		std::string address;
		std::ifstream myfile( lotname + ".txt");
		if (myfile.is_open())
		{
			
			while (getline(myfile, line))
			{
				feed_name = line;
				getline(myfile, line);
				address = line;
				getline(myfile, line);
				feed_list.push_back(new Feed(address, feed_name,line)); 
				
			}
			myfile.close();
		}

		else std::cout << "Unable to open file";

}


std::vector<Feed *>  Lot::getFeeds() {
	return feed_list;
}

void Lot::runFeeds() {
	
	int feed_size = feed_list.size();
	for (int j = 0; j < feed_size; j++) {
	feed_list[j]->doStuff();
	lotCount = lotCount + feed_list[j]->getTotal();
	//std::cout << Lot_name << lotCount << "\n";

	}
}
cv::String Lot::getName() {
	return  Lot_name;
}
int Lot::getCount() {
	//std::cout << Lot_name << lotCount << "\n";
	return lotCount;
	
}

cv::String Lot::getCap() {
	return capacity;
}
//returns lot id
cv::String Lot::getId() {
	return id;
}
