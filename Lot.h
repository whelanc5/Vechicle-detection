#pragma once
#ifndef MY_LOT
#define MY_LOT
#include<opencv2/core/core.hpp>
#include<iostream>
#include <fstream>
#include "Feed.h"


class Lot {


private:
	//feed pointer vector, holds pointers to the Feed objects
	std::vector<Feed *> feed_list; 
	//name of the lot
	cv::String Lot_name;
	//maximum capacity of the lot
	cv::String capacity;
	
	int lotCount = 0;
	std::string id;
	// function prototypes ////////////////////////////////////////////////////////////////////////////
	
public:
	//constructor for Lot, takes string for the name and string for the capacity, and a string for the lotid
	Lot(cv::String lotname, cv::String cap, cv::String lotid);
	void addFeed(Feed feed); // allows you to add feeds
	//allows to remove feeds
	bool removeFeedAt(int index); 
	//returns the feeds within the lot
	std::vector<Feed *> getFeeds();
	void runFeeds();//makes the feeds run
	//returns the name of the lot
	cv::String getName();
	//returns count for the lot
	int getCount();
	//returns capacity
	cv::String getCap();
	//returns lot id
	cv::String getId();
};
#endif
