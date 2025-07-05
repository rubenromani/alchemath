
import { Commodity, SpreadAnalysisParams, AnalysisData, SpreadDataPoint, YearlyMetrics } from '@/types/analysis';

// Mock commodities data
const mockCommodities: Commodity[] = [
  {
    symbol: 'ZC',
    name: 'Corn',
    availableMonths: ['March', 'May', 'July', 'September', 'December']
  },
  {
    symbol: 'ZS',
    name: 'Soybeans',
    availableMonths: ['January', 'March', 'May', 'July', 'August', 'September', 'November']
  },
  {
    symbol: 'ZW',
    name: 'Wheat',
    availableMonths: ['March', 'May', 'July', 'September', 'December']
  },
  {
    symbol: 'CL',
    name: 'Crude Oil',
    availableMonths: ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December']
  },
  {
    symbol: 'GC',
    name: 'Gold',
    availableMonths: ['February', 'April', 'June', 'August', 'October', 'December']
  }
];

// Generate mock spread data
const generateSpreadData = (startDate: Date, endDate: Date, year?: number): SpreadDataPoint[] => {
  const data: SpreadDataPoint[] = [];
  const currentDate = new Date(startDate);
  const end = new Date(endDate);
  
  let baseValue = -15 + Math.random() * 30; // Start with random base value
  const trend = (Math.random() - 0.5) * 0.1; // Random trend
  const volatility = 2 + Math.random() * 3; // Random volatility
  
  while (currentDate <= end) {
    const dayOfYear = currentDate.getMonth() * 30 + currentDate.getDate();
    const seasonality = Math.sin((dayOfYear / 365) * 2 * Math.PI) * 5;
    const randomWalk = (Math.random() - 0.5) * volatility;
    
    baseValue += trend + randomWalk * 0.1;
    const value = baseValue + seasonality + randomWalk;
    
    data.push({
      date: currentDate.toISOString().split('T')[0],
      value: parseFloat(value.toFixed(2)),
      year: year || currentDate.getFullYear()
    });
    
    currentDate.setDate(currentDate.getDate() + 1);
  }
  
  return data;
};

// Generate yearly metrics
const generateYearlyMetrics = (spreadData: SpreadDataPoint[], year: number): YearlyMetrics => {
  const values = spreadData.map(d => d.value);
  const returns = values.slice(1).map((val, i) => val - values[i]);
  
  const profitLoss = values[values.length - 1] - values[0];
  const maxDrawdown = Math.min(...values) - Math.max(...values.slice(0, values.indexOf(Math.min(...values))));
  const maxProfit = Math.max(...values) - Math.min(...values.slice(0, values.indexOf(Math.max(...values))));
  const standardDeviation = Math.sqrt(returns.reduce((sum, ret) => sum + ret * ret, 0) / returns.length);
  const avgReturn = returns.reduce((sum, ret) => sum + ret, 0) / returns.length;
  const sharpeRatio = avgReturn / standardDeviation || 0;
  const positiveReturns = returns.filter(r => r > 0);
  const negativeReturns = returns.filter(r => r < 0);
  const winRate = positiveReturns.length / returns.length;
  const avgWin = positiveReturns.reduce((sum, ret) => sum + ret, 0) / positiveReturns.length || 0;
  const avgLoss = negativeReturns.reduce((sum, ret) => sum + ret, 0) / negativeReturns.length || 0;
  
  return {
    year,
    profitLoss: parseFloat(profitLoss.toFixed(2)),
    maxDrawdown: parseFloat(maxDrawdown.toFixed(2)),
    maxProfit: parseFloat(maxProfit.toFixed(2)),
    standardDeviation: parseFloat(standardDeviation.toFixed(3)),
    sharpeRatio: parseFloat(sharpeRatio.toFixed(3)),
    totalReturn: parseFloat(profitLoss.toFixed(2)),
    winRate: parseFloat(winRate.toFixed(3)),
    avgWin: parseFloat(avgWin.toFixed(2)),
    avgLoss: parseFloat(avgLoss.toFixed(2))
  };
};

export const mockApiService = {
  // Simulate API delay
  delay: (ms: number) => new Promise(resolve => setTimeout(resolve, ms)),

  getCommodities: async (): Promise<Commodity[]> => {
    await mockApiService.delay(800);
    return mockCommodities;
  },

  runSpreadAnalysis: async (params: SpreadAnalysisParams): Promise<AnalysisData> => {
    await mockApiService.delay(2000); // Simulate longer analysis time
    
    const startDate = new Date(params.startDate);
    const endDate = new Date(params.endDate);
    const currentYear = new Date().getFullYear();
    
    // Generate current year data
    const currentYearData = generateSpreadData(startDate, endDate, currentYear);
    
    // Generate historical data for averages
    const historicalYears = [];
    for (let year = currentYear - 15; year < currentYear; year++) {
      const yearStart = new Date(year, startDate.getMonth(), startDate.getDate());
      const yearEnd = new Date(year, endDate.getMonth(), endDate.getDate());
      historicalYears.push({
        year,
        data: generateSpreadData(yearStart, yearEnd, year)
      });
    }
    
    // Calculate averages
    const calculateAverage = (years: number) => {
      const relevantYears = historicalYears.slice(-years);
      const averageData: SpreadDataPoint[] = [];
      
      if (relevantYears.length > 0) {
        const maxLength = Math.max(...relevantYears.map(y => y.data.length));
        
        for (let i = 0; i < maxLength; i++) {
          const values = relevantYears
            .map(y => y.data[i]?.value)
            .filter(v => v !== undefined);
          
          if (values.length > 0) {
            const avgValue = values.reduce((sum, val) => sum + val, 0) / values.length;
            const dateTemplate = currentYearData[i] || currentYearData[currentYearData.length - 1];
            
            averageData.push({
              date: dateTemplate.date,
              value: parseFloat(avgValue.toFixed(2)),
              year: currentYear
            });
          }
        }
      }
      
      return averageData;
    };
    
    // Generate yearly metrics
    const yearlyMetrics: YearlyMetrics[] = historicalYears.map(({ year, data }) => 
      generateYearlyMetrics(data, year)
    );
    
    // Create yearly spread data object
    const yearlySpreadData: { [year: number]: SpreadDataPoint[] } = {};
    historicalYears.forEach(({ year, data }) => {
      yearlySpreadData[year] = data;
    });
    
    const analysisData: AnalysisData = {
      params,
      currentYearData,
      averageData: {
        avg3Year: calculateAverage(3),
        avg5Year: calculateAverage(5),
        avg10Year: calculateAverage(10),
        avg15Year: calculateAverage(15)
      },
      yearlyMetrics,
      yearlySpreadData
    };
    
    console.log('Generated analysis data:', analysisData);
    return analysisData;
  }
};
