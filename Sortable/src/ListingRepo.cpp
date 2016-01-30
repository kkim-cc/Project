/// File name: ListingRepo.cpp
/// Class name: ListingRepo
/// Description: ListingRepo is the Repository class. 
/// Author: KiDuck Kim
/// Date: Jan.28, 2016
///

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include "../include/ListingRepo.h"
#include "../include/json/json.h"

ListingRepo::ListingRepo()
{
}

ListingRepo::~ListingRepo()
{
	if(!m_listingRepo.empty())
	{
		m_listingRepo.clear();
	}
}

bool ListingRepo::loadDataFromFile(std::string &filePath)
{
	bool bRet = false;
	std::string line;
	
	std::ifstream infile(filePath.c_str(), std::ifstream::in);
	
	while (std::getline(infile, line))
	{
		Listing listing(line);
		m_listingRepo.insert(std::pair<std::string, Listing>(listing.getManufacturer(), listing));
		bRet = true;
	}
		
	return bRet;
}

void ListingRepo::saveSearchResultToFile(std::string &filePath, Product &searchProduct)
{
	std::string manufacturer = Listing::makeStringLower(searchProduct.getManufacturer());
	std::string model = searchProduct.getModel();
	std::string family = searchProduct.getFamily();
	std::string productName = searchProduct.getProductName();
	std::string jsonListing;
	std::string listing_manufacturer;
	std::string listing_title;
	std::string listing_currency;
	std::string listing_price;
	int nResult = 0;
	
	
	// 1. search multimap by manufacturer.
	
	std::pair <std::multimap<std::string,Listing>::iterator, 
			   std::multimap<std::string,Listing>::iterator> ret;
   
    ret = m_listingRepo.equal_range(manufacturer);
	
	std::cout<<"searching product:"<<productName<<" model:"<<model<<" family:"<<family<<std::endl;
	
	if(productName.empty())
	{
		std::cout<<"product name is empty"<<std::endl;
		return;
	}
	
	// 2. create the json Output and store information 
	Json::Value root;
	root["product_name"] = Json::Value(productName);
	Json::Value array = Json::Value(Json::arrayValue);
	
	for (std::multimap<std::string,Listing>::iterator it = ret.first; it != ret.second; ++it)
	{
		if(it->second.search(manufacturer, model, family, productName))
		{
			// create node & append array
			listing_title = it->second.getTitle();
			listing_currency = it->second.getCurrency();
			listing_manufacturer = it->second.getManufacturer();
			listing_price = it->second.getTitle();
			Json::Value node;
			node["title"] = Json::Value(listing_title);
			node["manufacturer"] = Json::Value(listing_manufacturer);
			node["currency"] = Json::Value(listing_currency);
			node["price"] = Json::Value(listing_price);
			
			array.append(node);
			nResult++;
		}
	}
	// print the result of searching for logging
	std::cout<<"searching Result:"<<nResult<<std::endl;
	
	root["listings"] = array;
	
	// 3. make JSON string
	Json::FastWriter writer;
	jsonListing = writer.write(root);
	
	// append json string data to file
	std::ofstream outfile;
	
	outfile.open(filePath.c_str(), std::ofstream::app);
	outfile << jsonListing; 
	outfile.close();
}

int ListingRepo::count()
{
	return (int)m_listingRepo.size();
}

void ListingRepo::clear()
{
	if(!m_listingRepo.empty())
	{
		m_listingRepo.clear();
	}
}
