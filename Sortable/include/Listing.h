/// <Listing>
/// Class name: Listing
/// Description: Listing is the class that storing information of Listing.
/// Author: KiDuck Kim
/// Date: Jan.28, 2016
/// </Listing>
///
#ifndef CPP_LISTING_H_INCLUDED
# define CPP_LISTING_H_INCLUDED
# include <string>

/** \brief Listing class.
 * This class hold the information of Listing.
 *  - title
 *  - maufacturer
 *  - currency
 *  - price
 * 
 * Provide default get/set methods.
 * Provide variant Creator methods like as JSON string.
 */ 
class Listing
{
public:
	/** \brief Constructs a Listing default.
	 */
	Listing();
	
	/** \brief Constructs a Listing with initial JsonCpp string.
	 * \param json_string UTF-8 encoded string containing the document to read.
	 */
	Listing(const std::string &json_string);
	
	/** \brief Constructs a Listing with initial values
	 * \param title 		title of listing
	 * \param manufacturer	manufacturer of product
	 * \param currency		currency
	 * \param price			price of listing
	 */
	Listing(std::string title, 
		 std::string manufacturer, 
		 std::string currency,
		 std::string price);
	
	/** \brief Deconstructs a Listing default.
	 */
	~Listing();
	
	/** \brief get information of listing.
	 * \return string information of each data of listing
	 */
	std::string getTitle();
	std::string getManufacturer();
	std::string getCurrency();
	std::string getPrice();
	
	/** \brief set title information of listing
	 * \param title title
	 * \return \c true if the information was successfully saved, \c false if an error occurred.
	 */
	bool setTitle(std::string &title);

	/** \brief set manufacturer information of listing
	 * \param manufacturer manufacturer
	 * \return \c true if the information was successfully saved, \c false if an error occurred.
	 */
	bool setManufacturer(std::string &manufacturer);

	/** \brief set currency information of listing
	 * \param currency currency
	 * \return \c true if the information was successfully saved, \c false if an error occurred.
	 */
	bool setCurrency(std::string &currency);

	/** \brief set price information of listing
	 * \param price price
	 * \return \c true if the information was successfully saved, \c false if an error occurred.
	 */
	bool setPrice(std::string &price);
	
	/** \brief search listing by given information of product 
	 * \param keyManufacturer 	manufacturer of product
	 * \param keyModel 			model of product
	 * \param keyFamily 		family of product
	 * \param keyProductName	name of product
	 * \return \c true if the result exist, \c false if the result does not exist.
	 */
	bool search(std::string &keyManufacturer, 
		std::string &keyModel,
		std::string &keyFamily,
		std::string &keyProductName);
	
	/** \brief return lower string 
	 * This function used for searching
	 * \param src 	source string
	 * \return string the lower string
	 */
	static std::string makeStringLower(std::string src);
	
private:
	std::string m_title;
	std::string m_manufacturer;
	std::string m_currency;
	std::string m_price;
};

#endif // CPP_LISTING_H_INCLUDED