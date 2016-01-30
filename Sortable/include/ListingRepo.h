/// <ListingRepo>
/// Class name: ListingRepo
/// Description: ListingRepo is the Repository class of Listing class. 
/// Author: KiDuck Kim
/// Date: Jan.28, 2016
/// </ListingRepo> 
///
#ifndef CPP_LISTINGREPO_H_INCLUDED
# define CPP_LISTINGREPO_H_INCLUDED

#include <string>
#include <map>

#include "Listing.h"
#include "Product.h"
/** \brief ListingRepo class.
 * This class hold the repository multimap of Listings.
 */ 
class ListingRepo
{
public:
	/** \brief Constructs a ListingRepo.
	 */
	ListingRepo();
	
	/** \brief Deconstructs a ListingRepo 
	 *  remove all array of products
	 */
	~ListingRepo();
	
	/** \brief Load Listing data from file
	 * \param filePath file path of the input data .
	 * \return \c true if the data was successfully loaded, \c false if an error occurred.
	 */
	bool loadDataFromFile(std::string &filePath);
	
	/** \brief Load Listing data from file
	 *  excute the following procedures for searching
	 *   - narrow the range of search boundaries using multimap.equal_range()
	 *   - search title by model of product.
	 *   - save the result information to JsonCpp class
	 *   - append the searched inforation to given file by using Json::FastWriter class
	 * \param filePath 		file path of the output data .
	 * \param searchProduct the information of Product
	 */
	void saveSearchResultToFile(std::string &filePath, Product &searchProduct);
	
	/** \brief Get total count of Listing
	 * \return the number of Listing.
	 */
	int count();
	
	/** \brief clear all holding Listing data
	 */
	void clear();
private:
	std::multimap<std::string, Listing> m_listingRepo;
};

#endif // CPP_LISTINGREPO_H_INCLUDED 
