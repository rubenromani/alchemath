from typing import List
from app.models.commodity import Commodity
import logging

logger = logging.getLogger(__name__)

class CommodityService:
    """Service for managing commodity data and operations."""
    
    # Mock data - in production this would come from database
    _mock_commodities = [
        Commodity(
            symbol="ZC",
            name="Corn",
            available_months=["March", "May", "July", "September", "December"]
        ),
        Commodity(
            symbol="ZS",
            name="Soybeans",
            available_months=["January", "March", "May", "July", "August", "September", "November"]
        ),
        Commodity(
            symbol="ZW",
            name="Wheat",
            available_months=["March", "May", "July", "September", "December"]
        ),
        Commodity(
            symbol="CL",
            name="Crude Oil",
            available_months=["January", "February", "March", "April", "May", "June", 
                            "July", "August", "September", "October", "November", "December"]
        ),
        Commodity(
            symbol="GC",
            name="Gold",
            available_months=["February", "April", "June", "August", "October", "December"]
        )
    ]
    
    async def get_all_commodities(self) -> List[Commodity]:
        """Get all available commodities."""
        logger.info("Fetching all commodities")
        # Simulate some processing time
        import asyncio
        await asyncio.sleep(0.1)
        
        return self._mock_commodities
    
    async def get_commodity_by_symbol(self, symbol: str) -> Commodity:
        """Get commodity by symbol."""
        logger.info(f"Fetching commodity with symbol: {symbol}")
        
        for commodity in self._mock_commodities:
            if commodity.symbol.upper() == symbol.upper():
                return commodity
                
        raise ValueError(f"Commodity with symbol '{symbol}' not found")
    
    async def validate_commodity_months(self, symbol: str, months: List[str]) -> bool:
        """Validate that the given months are available for the commodity."""
        try:
            commodity = await self.get_commodity_by_symbol(symbol)
            return all(month in commodity.available_months for month in months)
        except ValueError:
            return False

# Singleton instance
commodity_service = CommodityService()