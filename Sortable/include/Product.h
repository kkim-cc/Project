/// <Product>
/// Class name: Product
/// Description: Product is the class that storing information of product.
/// Author: KiDuck Kim
/// Date: Jan.28, 2016
/// </Product>
///
#ifndef CPP_PRODUCT_H_INCLUDED
# define CPP_PRODUCT_H_INCLUDED

# include <string>

/** \brief Product class.
 * This class hold the information of Product.
 *  - product name
 *  - maufacturer
 *  - model
 *  - announced date
 * Provide default get/set methods.
 * Provide variant Creator methods like as JSON string.
 */ 
class Product
{
public:
	/** \brief Constructs a Product with initial JsonCpp string.
	 * \param json_string UTF-8 encoded string containing the document to read.
	 */
	Product(const std::string &json_string);
	
	/** \brief Constructs a Product default.
	 */
	Product();
	
	/** \brief Constructs a Product with initial values
	 * \param productName 	product name
	 * \param manufacturer	manufacturer of product
	 * \param model			model of product
	 * \param announcedDate	announced date
	 * \param family		family of product
	 */
	Product(std::string &productName, 
		 std::string &manufacturer, 
		 std::string &model,
		 std::string &announcedDate,
		 std::string &family
	);
	
	/** \brief Deconstructs a product default.
	 */
	~Product();
	
	/** \brief get information of product.
	 * \return string information of each data of product
	 */
	std::string getProductName();
	std::string getManufacturer();
	std::string getModel();
	std::string getAnnouncedDate();
	std::string getFamily();
	
	/** \brief set product name information of product.
	 * \param productName product name
	 * \return \c true if the information was successfully saved, \c false if an error occurred.
	 */
	bool setProductName(std::string &productName);
	
	/** \brief set manufacturer information of product.
	 * \param manufacturer manufacturer of product
	 * \return \c true if the information was successfully saved, \c false if an error occurred.
	 */
	bool setManufacturer(std::string &manufacturer);
	
	/** \brief set model information of product.
	 * \param model model of product
	 * \return \c true if the information was successfully saved, \c false if an error occurred.
	 */
	bool setModel(std::string &model);
	
	/** \brief set announcedDate information of product.
	 * \param announcedDate announced date
	 * \return \c true if the information was successfully saved, \c false if an error occurred.
	 */
	bool setAnnouncedDate(std::string &announcedDate);
	
	/** \brief set family information of product.
	 * \param family family of product
	 * \return \c true if the information was successfully saved, \c false if an error occurred.
	 */
	bool setFamily(std::string &family);
	
private:
	std::string m_product_name;
	std::string m_manufacturer;
	std::string m_model;
	std::string m_announced_date;
	std::string m_family;
};



#endif // CPP_PRODUCT_H_INCLUDED