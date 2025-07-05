from pydantic import BaseModel, Field
from typing import List, Dict, Optional
from datetime import date

class SpreadDataPoint(BaseModel):
    date: str = Field(..., description="Date in YYYY-MM-DD format")
    value: float = Field(..., description="Spread value")
    year: int = Field(..., description="Year of the data point")

class YearlyMetrics(BaseModel):
    year: int
    profit_loss: float = Field(..., alias="profitLoss")
    max_drawdown: float = Field(..., alias="maxDrawdown")
    max_profit: float = Field(..., alias="maxProfit")
    standard_deviation: float = Field(..., alias="standardDeviation")
    sharpe_ratio: float = Field(..., alias="sharpeRatio")
    total_return: float = Field(..., alias="totalReturn")
    win_rate: float = Field(..., alias="winRate")
    avg_win: float = Field(..., alias="avgWin")
    avg_loss: float = Field(..., alias="avgLoss")
    
    class Config:
        populate_by_name = True

class AverageData(BaseModel):
    avg_3_year: List[SpreadDataPoint] = Field(..., alias="avg3Year")
    avg_5_year: List[SpreadDataPoint] = Field(..., alias="avg5Year")
    avg_10_year: List[SpreadDataPoint] = Field(..., alias="avg10Year")
    avg_15_year: List[SpreadDataPoint] = Field(..., alias="avg15Year")
    
    class Config:
        populate_by_name = True

class SpreadAnalysisParams(BaseModel):
    commodity: str = Field(..., description="Commodity symbol")
    month1: str = Field(..., description="First contract month")
    month2: str = Field(..., description="Second contract month")
    start_date: str = Field(..., alias="startDate", description="Analysis start date (YYYY-MM-DD)")
    end_date: str = Field(..., alias="endDate", description="Analysis end date (YYYY-MM-DD)")
    
    class Config:
        populate_by_name = True
        json_schema_extra = {
            "example": {
                "commodity": "ZC",
                "month1": "March",
                "month2": "May",
                "startDate": "2020-01-01",
                "endDate": "2023-12-31"
            }
        }

class AnalysisData(BaseModel):
    params: SpreadAnalysisParams
    current_year_data: List[SpreadDataPoint] = Field(..., alias="currentYearData")
    average_data: AverageData = Field(..., alias="averageData")
    yearly_metrics: List[YearlyMetrics] = Field(..., alias="yearlyMetrics")
    yearly_spread_data: Dict[str, List[SpreadDataPoint]] = Field(..., alias="yearlySpreadData")
    
    class Config:
        populate_by_name = True