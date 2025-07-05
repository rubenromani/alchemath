from typing import List, Dict
import asyncio
import math
import random
from datetime import datetime, timedelta
from app.models.analysis import (
    SpreadAnalysisParams, AnalysisData, SpreadDataPoint, 
    YearlyMetrics, AverageData
)
from app.services.commodity_service import commodity_service
import logging

logger = logging.getLogger(__name__)

class AnalysisService:
    """Service for performing spread analysis operations."""
    
    def __init__(self):
        self.commodity_service = commodity_service
    
    async def run_spread_analysis(self, params: SpreadAnalysisParams) -> AnalysisData:
        """
        Run spread analysis with given parameters.
        This is a mock implementation that will be replaced by C++ engine calls.
        """
        logger.info(f"Starting spread analysis for {params.commodity} {params.month1}-{params.month2}")
        
        # Validate commodity and months
        await self._validate_analysis_params(params)
        
        # Simulate processing time
        await asyncio.sleep(2.0)
        
        # Generate mock data (this will be replaced by C++ engine)
        start_date = datetime.strptime(params.start_date, "%Y-%m-%d")
        end_date = datetime.strptime(params.end_date, "%Y-%m-%d")
        current_year = datetime.now().year
        
        # Generate current year data
        current_year_data = await self._generate_spread_data(start_date, end_date, current_year)
        
        # Generate historical data for averages
        historical_data = {}
        yearly_metrics = []
        
        for year in range(current_year - 15, current_year):
            year_start = start_date.replace(year=year)
            year_end = end_date.replace(year=year)
            year_data = await self._generate_spread_data(year_start, year_end, year)
            historical_data[str(year)] = year_data
            yearly_metrics.append(await self._calculate_yearly_metrics(year_data, year))
        
        # Calculate averages
        average_data = await self._calculate_averages(historical_data, current_year_data)
        
        result = AnalysisData(
            params=params,
            current_year_data=current_year_data,
            average_data=average_data,
            yearly_metrics=yearly_metrics,
            yearly_spread_data=historical_data
        )
        
        logger.info(f"Completed spread analysis for {params.commodity}")
        return result
    
    async def _validate_analysis_params(self, params: SpreadAnalysisParams):
        """Validate analysis parameters."""
        # Check if commodity exists
        await self.commodity_service.get_commodity_by_symbol(params.commodity)
        
        # Check if months are valid for this commodity
        months_valid = await self.commodity_service.validate_commodity_months(
            params.commodity, [params.month1, params.month2]
        )
        if not months_valid:
            raise ValueError(f"Invalid months {params.month1}, {params.month2} for commodity {params.commodity}")
        
        # Check date range
        start_date = datetime.strptime(params.start_date, "%Y-%m-%d")
        end_date = datetime.strptime(params.end_date, "%Y-%m-%d")
        
        if start_date >= end_date:
            raise ValueError("Start date must be before end date")
        
        if end_date > datetime.now():
            raise ValueError("End date cannot be in the future")
    
    async def _generate_spread_data(self, start_date: datetime, end_date: datetime, year: int) -> List[SpreadDataPoint]:
        """Generate mock spread data for the given date range."""
        data = []
        current_date = start_date
        
        # Random walk parameters
        base_value = -15 + random.random() * 30
        trend = (random.random() - 0.5) * 0.1
        volatility = 2 + random.random() * 3
        
        while current_date <= end_date:
            # Add seasonality
            day_of_year = current_date.timetuple().tm_yday
            seasonality = math.sin((day_of_year / 365) * 2 * math.pi) * 5
            
            # Random walk
            random_walk = (random.random() - 0.5) * volatility
            base_value += trend + random_walk * 0.1
            
            value = base_value + seasonality + random_walk
            
            data.append(SpreadDataPoint(
                date=current_date.strftime("%Y-%m-%d"),
                value=round(value, 2),
                year=year
            ))
            
            current_date += timedelta(days=1)
        
        return data
    
    async def _calculate_yearly_metrics(self, data: List[SpreadDataPoint], year: int) -> YearlyMetrics:
        """Calculate metrics for a year of data."""
        values = [point.value for point in data]
        returns = [values[i] - values[i-1] for i in range(1, len(values))]
        
        profit_loss = values[-1] - values[0] if values else 0
        max_value = max(values) if values else 0
        min_value = min(values) if values else 0
        max_drawdown = min_value - max_value if values else 0
        max_profit = max_value - min_value if values else 0
        
        # Standard deviation
        if returns:
            mean_return = sum(returns) / len(returns)
            variance = sum((r - mean_return) ** 2 for r in returns) / len(returns)
            standard_deviation = math.sqrt(variance)
        else:
            mean_return = 0
            standard_deviation = 0
        
        # Sharpe ratio (simplified)
        sharpe_ratio = mean_return / standard_deviation if standard_deviation > 0 else 0
        
        # Win rate
        positive_returns = [r for r in returns if r > 0]
        negative_returns = [r for r in returns if r < 0]
        win_rate = len(positive_returns) / len(returns) if returns else 0
        
        avg_win = sum(positive_returns) / len(positive_returns) if positive_returns else 0
        avg_loss = sum(negative_returns) / len(negative_returns) if negative_returns else 0
        
        return YearlyMetrics(
            year=year,
            profit_loss=round(profit_loss, 2),
            max_drawdown=round(max_drawdown, 2),
            max_profit=round(max_profit, 2),
            standard_deviation=round(standard_deviation, 3),
            sharpe_ratio=round(sharpe_ratio, 3),
            total_return=round(profit_loss, 2),
            win_rate=round(win_rate, 3),
            avg_win=round(avg_win, 2),
            avg_loss=round(avg_loss, 2)
        )
    
    async def _calculate_averages(self, historical_data: Dict[str, List[SpreadDataPoint]], 
                                current_data: List[SpreadDataPoint]) -> AverageData:
        """Calculate historical averages."""
        years = sorted([int(year) for year in historical_data.keys()])
        current_year = datetime.now().year
        
        def calculate_average_for_years(num_years: int) -> List[SpreadDataPoint]:
            relevant_years = years[-num_years:] if len(years) >= num_years else years
            if not relevant_years:
                return []
            
            # Get max length for alignment
            max_length = max(len(historical_data[str(year)]) for year in relevant_years)
            averaged_data = []
            
            for i in range(min(len(current_data), max_length)):
                values = []
                for year in relevant_years:
                    year_data = historical_data[str(year)]
                    if i < len(year_data):
                        values.append(year_data[i].value)
                
                if values:
                    avg_value = sum(values) / len(values)
                    averaged_data.append(SpreadDataPoint(
                        date=current_data[i].date if i < len(current_data) else f"{current_year}-12-31",
                        value=round(avg_value, 2),
                        year=current_year
                    ))
            
            return averaged_data
        
        return AverageData(
            avg_3_year=calculate_average_for_years(3),
            avg_5_year=calculate_average_for_years(5),
            avg_10_year=calculate_average_for_years(10),
            avg_15_year=calculate_average_for_years(15)
        )

# Singleton instance
analysis_service = AnalysisService()