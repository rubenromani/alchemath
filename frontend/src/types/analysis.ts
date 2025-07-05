export interface Commodity {
  symbol: string;
  name: string;
  available_months: string[]; // Updated from availableMonths to match backend
}

export interface SpreadDataPoint {
  date: string;
  value: number;
  year: number;
}

export interface YearlyMetrics {
  year: number;
  profitLoss: number;     // Frontend camelCase
  maxDrawdown: number;    // Frontend camelCase  
  maxProfit: number;      // Frontend camelCase
  standardDeviation: number; // Frontend camelCase
  sharpeRatio: number;    // Frontend camelCase
  totalReturn: number;    // Frontend camelCase
  winRate: number;        // Frontend camelCase
  avgWin: number;         // Frontend camelCase
  avgLoss: number;        // Frontend camelCase
}

export interface SpreadAnalysisParams {
  commodity: string;
  month1: string;
  month2: string;
  startDate: string;      // Frontend camelCase
  endDate: string;        // Frontend camelCase
}

export interface AnalysisData {
  params: SpreadAnalysisParams;
  currentYearData: SpreadDataPoint[];    // Frontend camelCase
  averageData: {                         // Frontend camelCase
    avg3Year: SpreadDataPoint[];         // Frontend camelCase
    avg5Year: SpreadDataPoint[];         // Frontend camelCase
    avg10Year: SpreadDataPoint[];        // Frontend camelCase
    avg15Year: SpreadDataPoint[];        // Frontend camelCase
  };
  yearlyMetrics: YearlyMetrics[];        // Frontend camelCase
  yearlySpreadData: {                    // Frontend camelCase
    [year: number]: SpreadDataPoint[];
  };
}