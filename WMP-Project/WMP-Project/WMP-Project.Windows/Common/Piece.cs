// FILE				: Piece.cs
// PROJECT			: Windows Programming(PROG2120) – Assignment 7: Windows Surface Programming
// PROGRAMMER(S)    : KiDuck Kim(DUCKIE), Robin Dagenais(ROB)
// FIRST VERSION	: NOV25-2014
// DESCRIPTION		: Create and manage the puzzle image sections
//     
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml.Shapes;
using Windows.Data.Json;

namespace WMP_Project.Common
{
    class Piece
    {
        private const string nOrderKey = "nOrder";
        private const string PieceKey = "Piece";
        private int nOrder;
        private Rectangle onePiece;

		
		/// <summary>
		///  Default constructor 
		/// </summary>
        public Piece()
        {
            nOrder = 0;
        }

		
		/// <summary>
		///  Constructor that  initializes with JSON object
		/// </summary>
		/// <param name="JsonObject"></param>
        public Piece(JsonObject jsonObject)
        {
            JsonObject PieceObject = jsonObject.GetNamedObject(PieceKey, null);
            if (PieceObject != null)
            {
                nOrder = (int)PieceObject.GetNamedNumber(nOrderKey, 0);
                onePiece = new Rectangle();
            }
        }

		
		/// <summary>
		///  Creates a JSON object for passing data
		/// </summary>
        /// <returns>JsonObject</returns>
        public JsonObject ToJsonObject()
        {
            JsonObject PieceObject = new JsonObject();
            PieceObject.SetNamedValue(nOrderKey, JsonValue.CreateNumberValue((double)nOrder));

            JsonObject jsonObject = new JsonObject();
            jsonObject.SetNamedValue(PieceKey, PieceObject);

            return jsonObject;
        }

		
		/// <summary>
		///  Sets/gets the order
		/// </summary>
        /// <returns>int</returns>
        public int NOrder 
        { 
            set { nOrder = value; } 
            get { return nOrder; } 
        }
		
		
		/// <summary>
		/// Gets/Sets the value of a puzzle piece
		/// </summary>
        /// <returns>Rectangle</returns>		
        public Rectangle OnePiece 
        {
            set { onePiece = value; }  
            get { return onePiece; } 
        }
    }
}
