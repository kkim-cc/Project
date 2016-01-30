/// <ProductRepo>
/// Class name: ProductRepo
/// Description: ProductRepo is the Repository class. 
/// Author: KiDuck Kim
/// Date: Jan.28, 2016
/// </ProductRepo> 
///
#ifndef CPP_PRODUCTREPO_H_INCLUDED
# define CPP_PRODUCTREPO_H_INCLUDED

#include <string>
#include <vector>

#include "Product.h"
/** \brief ProductRepo class.
 * This class hold the repository array of Products.
 */ 
class ProductRepo
{
public:
	/** \brief Constructs a ProductRepo.
	 */
	ProductRepo();
	
	/** \brief Deconstructs a ProductRepo 
	 *  remove all array of products
	 */
	~ProductRepo();
	
	/** \brief Load product data from file
	 * \param filePath file path of the input data .
	 * \return \c true if the data was successfully loaded, \c false if an error occurred.
	 */
	bool loadDataFromFile(std::string &filePath);

	/** \brief Get total count of products
	 * \return the number of products.
	 */
	int count();

	/** \brief get information of Product class at given position
	 * \param nPos the position of array
	 * \return Product class with information, if nPos was wrong, then return empty class
	 */
	Product getProduct(int nPos);

	/** \brief clear all holding product data
	 */
	void clear();
private:
	std::vector<Product> m_productRepo;
};

#endif // CPP_PRODUCTREPO_H_INCLUDED