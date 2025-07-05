from fastapi import APIRouter, HTTPException, BackgroundTasks
from app.models.analysis import SpreadAnalysisParams, AnalysisData
from app.services.analysis_service import analysis_service
import logging

logger = logging.getLogger(__name__)
router = APIRouter(prefix="/analysis", tags=["analysis"])

@router.post("/spread", response_model=AnalysisData)
async def run_spread_analysis(params: SpreadAnalysisParams):
    """Run spread analysis with given parameters."""
    try:
        logger.info(f"Received spread analysis request: {params.commodity} {params.month1}-{params.month2}")
        result = await analysis_service.run_spread_analysis(params)
        logger.info(f"Successfully completed spread analysis for {params.commodity}")
        return result
    except ValueError as e:
        logger.warning(f"Validation error in spread analysis: {e}")
        raise HTTPException(status_code=400, detail=str(e))
    except Exception as e:
        logger.error(f"Error in spread analysis: {e}")
        raise HTTPException(status_code=500, detail="Internal server error")

@router.get("/health")
async def analysis_health():
    """Health check for analysis service."""
    return {"status": "healthy", "service": "analysis"}