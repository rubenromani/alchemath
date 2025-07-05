from fastapi import APIRouter, HTTPException, Depends
from typing import List
from app.models.commodity import Commodity
from app.models.response import APIResponse
from app.services.commodity_service import commodity_service
import logging

logger = logging.getLogger(__name__)
router = APIRouter(prefix="/commodities", tags=["commodities"])

@router.get("/", response_model=List[Commodity])
async def get_commodities():
    """Get all available commodities."""
    try:
        commodities = await commodity_service.get_all_commodities()
        logger.info(f"Retrieved {len(commodities)} commodities")
        return commodities
    except Exception as e:
        logger.error(f"Error fetching commodities: {e}")
        raise HTTPException(status_code=500, detail="Internal server error")

@router.get("/{symbol}", response_model=Commodity)
async def get_commodity(symbol: str):
    """Get commodity by symbol."""
    try:
        commodity = await commodity_service.get_commodity_by_symbol(symbol)
        return commodity
    except ValueError as e:
        logger.warning(f"Commodity not found: {e}")
        raise HTTPException(status_code=404, detail=str(e))
    except Exception as e:
        logger.error(f"Error fetching commodity {symbol}: {e}")
        raise HTTPException(status_code=500, detail="Internal server error")