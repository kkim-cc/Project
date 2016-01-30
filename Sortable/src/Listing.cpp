/// File name: Listing.cpp
/// Class name: Listing
/// Description: Listing is the class that storing information of Listing 
/// Author: KiDuck Kim
/// Date: Jan.28, 2016
///

#include <algorithm>
#include <string>
#include "../include/Listing.h"
# include "../include/json/json.h"

Listing::Listing()
{
	m_currency = "";
	m_manufacturer = "";
	m_price = "";
	m_title = "";
}

Listing::Listing(const std::string &json_string)
{
	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(json_string, root, false);
	
	if(parsedSuccess)
	{
		const Json::Value title = root["title"];
		const Json::Value manufacturer = root["manufacturer"];
		const Json::Value currency = root["currency"];
		const Json::Value price = root["price"];
		m_title = title.asString();
		m_manufacturer = makeStringLower(manufacturer.asString());
		m_currency = currency.asString();
		m_price = price.asString();
	}
	else
	{
		m_currency = "";
		m_manufacturer = "";
		m_price = "";
		m_title = "";	
	}
}
Listing::Listing(std::string title, 
				std::string manufacturer, 
				std::string currency,
				std::string price)
{
	m_currency = currency;
	m_manufacturer = makeStringLower(manufacturer);
	m_price = price;
	m_title = title;	
}

Listing::~Listing()
{
}

std::string Listing::makeStringLower(std::string src)
{
	std::string data = src;
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}

bool Listing::search(std::string &keyManufacturer, 
					std::string &keyModel,
				    std::string &keyFamily,
					std::string &keyProductName)
{
	bool bRet = false;
	
	std::string title = makeStringLower(m_title);
	// 
	// Change 'model' key for precision of search
	// - insert ' ' at heading and tailing
	//   prevent the result of search like as
	//   model:"TX10" & title has "TX100V"
	//
	std::string model = " " + makeStringLower(keyModel) + " ";
	
	
	if(title.find(model) != std::string::npos)
	{
		bRet = true;
	}
	return bRet;
}

std::string Listing::getTitle()
{
	return m_title;
}

std::string Listing::getManufacturer()
{
	return m_manufacturer;
}

std::string Listing::getCurrency()
{
	return m_currency;
}

std::string Listing::getPrice()
{
	return m_price;
}

bool Listing::setTitle(std::string &title)
{
	if(title.empty())
		return false;
	m_title = title;
	return true;
}

bool Listing::setManufacturer(std::string &manufacturer)
{
	m_manufacturer = manufacturer;
	return true;
}

bool Listing::setCurrency(std::string &currency)
{
	m_currency = currency;
	return true;
}

bool Listing::setPrice(std::string &price)
{
	m_price = price;
	return true;
}
