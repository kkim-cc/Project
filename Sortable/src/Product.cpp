/// File name: Product.cpp
/// Class name: Product
/// Description: Product is the class that storing information of product 
/// Author: KiDuck Kim
/// Date: Jan.28, 2016

#include "../include/Product.h"
#include "../include/json/json.h"

Product::Product(const std::string &json_string)
{
	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(json_string, root, false);
	
	if(parsedSuccess)
	{
		const Json::Value product_name = root["product_name"];
		const Json::Value manufacturer = root["manufacturer"];
		const Json::Value model = root["model"];
		const Json::Value announced_date = root["announced-date"];
		const Json::Value family = root["family"];
		m_product_name = product_name.asString();
		m_manufacturer = manufacturer.asString();
		m_model = model.asString();
		m_announced_date = announced_date.asString();
		m_family = family.asString();
	}
	else
	{
		m_product_name = "";
		m_manufacturer = "";
		m_model = "";
		m_announced_date = "";
		m_family = "";
	}
}

Product::Product()
{
	m_product_name = "";
	m_manufacturer = "";
	m_model = "";
	m_announced_date = "";
	m_family = "";
}

Product::Product(std::string &productName, 
				 std::string &manufacturer, 
				 std::string &model,
				 std::string &announcedDate,
				 std::string &family)
{
	m_product_name = productName;
	m_manufacturer = manufacturer;
	m_model = model;
	m_announced_date = announcedDate;
	m_family = family;
}

Product::~Product()
{
}

std::string Product::getProductName()
{
	return m_product_name;
}

std::string Product::getManufacturer()
{
	return m_manufacturer;
}

std::string Product::getModel()
{
	return m_model;
}

std::string Product::getAnnouncedDate()
{
	return m_announced_date;
}

std::string Product::getFamily()
{
	return m_family;
}

bool Product::setProductName(std::string &productName)
{
	if(productName.empty())
		return false;
	m_product_name = productName;
	return true;
}

bool Product::setManufacturer(std::string &manufacturer)
{
	m_manufacturer = manufacturer;
	return true;
}

bool Product::setModel(std::string &model)
{
	m_model = model;
	return true;
}

bool Product::setAnnouncedDate(std::string &announcedDate)
{
	m_announced_date = announcedDate;
	return true;
}

bool Product::setFamily(std::string &family)
{
	m_family = family;
	return true;
}