/// File name: CodingChallenge.cpp
/// 
/// Description: This is Main program that handling all classes
/// Author: KiDuck Kim
/// Date: Jan.28, 2016
/// 

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>

#include "../include/ListingRepo.h"
#include "../include/ProductRepo.h"

using namespace std;
int main(int argc, char **argv)
{
	ListingRepo listingRepo;
	ProductRepo productRepo;
	int productCount = 0, listingCount = 0;
	Product product;
	
	string filePath = "Data/products.txt";
	
	// 1. load products data from ./Data/product.txt
	if(!productRepo.loadDataFromFile(filePath))
	{
		cout<< "Load error from Data/products.txt" << endl;
		return -1;
	}
	
	// print the result for logging
	productCount = productRepo.count();
	cout<< "Loaded product count: " << productCount << endl;
	
	
	// 2. load listings data from ./Data/listing.txt
	filePath = "Data/listings.txt";
	if(!listingRepo.loadDataFromFile(filePath))
	{
		cout<< "Load error from Data/listings.txt" << endl;
		return -1;
	}
	
	// print the result for logging
	listingCount = listingRepo.count();
	cout<< "Loaded Listing count: " << listingCount << endl;
	
	
	// 3. create the output file.
	ofstream outfile;
	filePath = "Data/result.txt";
	outfile.open(filePath.c_str(), std::ofstream::out | std::ofstream::trunc);
	outfile.close();
	
	// 4. search and save the result of searched information.
	for(int k=0; k<productCount; k++)
	{
		product = productRepo.getProduct(k);
		listingRepo.saveSearchResultToFile(filePath, product);
	}
	
	return 0;
}