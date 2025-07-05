
import { useState } from 'react';
import { Card, CardContent, CardHeader, CardTitle } from '@/components/ui/card';
import { Table, TableBody, TableCell, TableHead, TableHeader, TableRow } from '@/components/ui/table';
import { YearlyMetrics } from '@/types/analysis';
import { ChevronUp, ChevronDown } from 'lucide-react';

interface MetricsTableProps {
  metrics: YearlyMetrics[];
}

type SortKey = keyof YearlyMetrics;
type SortOrder = 'asc' | 'desc';

export const MetricsTable = ({ metrics }: MetricsTableProps) => {
  const [sortKey, setSortKey] = useState<SortKey>('year');
  const [sortOrder, setSortOrder] = useState<SortOrder>('desc');

  const handleSort = (key: SortKey) => {
    if (sortKey === key) {
      setSortOrder(sortOrder === 'asc' ? 'desc' : 'asc');
    } else {
      setSortKey(key);
      setSortOrder('desc');
    }
  };

  const sortedMetrics = [...metrics].sort((a, b) => {
    const aValue = a[sortKey];
    const bValue = b[sortKey];
    
    if (sortOrder === 'asc') {
      return aValue < bValue ? -1 : aValue > bValue ? 1 : 0;
    } else {
      return aValue > bValue ? -1 : aValue < bValue ? 1 : 0;
    }
  });

  const formatValue = (key: SortKey, value: number) => {
    switch (key) {
      case 'year':
        return value.toString();
      case 'profitLoss':
      case 'maxDrawdown':
      case 'maxProfit':
      case 'totalReturn':
      case 'avgWin':
      case 'avgLoss':
        return `${value > 0 ? '+' : ''}${value.toFixed(2)}`;
      case 'standardDeviation':
      case 'sharpeRatio':
        return value.toFixed(3);
      case 'winRate':
        return `${(value * 100).toFixed(1)}%`;
      default:
        return value.toFixed(2);
    }
  };

  const getValueColor = (key: SortKey, value: number) => {
    if (key === 'year') return 'text-white';
    if (key === 'winRate') return value >= 0.5 ? 'text-green-400' : 'text-red-400';
    if (key === 'maxDrawdown') return value < 0 ? 'text-red-400' : 'text-green-400';
    return value >= 0 ? 'text-green-400' : 'text-red-400';
  };

  const SortIcon = ({ column }: { column: SortKey }) => {
    if (sortKey !== column) return null;
    return sortOrder === 'asc' ? (
      <ChevronUp className="w-4 h-4 inline ml-1" />
    ) : (
      <ChevronDown className="w-4 h-4 inline ml-1" />
    );
  };

  return (
    <Card className="bg-slate-800 border-slate-700">
      <CardHeader>
        <CardTitle className="text-white">Year-by-Year Metrics</CardTitle>
        <p className="text-slate-300 text-sm">
          Click column headers to sort • Positive values in green, negative in red
        </p>
      </CardHeader>
      <CardContent>
        <div className="overflow-x-auto">
          <Table>
            <TableHeader>
              <TableRow className="border-slate-600">
                <TableHead 
                  className="text-slate-300 cursor-pointer hover:text-white"
                  onClick={() => handleSort('year')}
                >
                  Year <SortIcon column="year" />
                </TableHead>
                <TableHead 
                  className="text-slate-300 cursor-pointer hover:text-white"
                  onClick={() => handleSort('profitLoss')}
                >
                  P&L <SortIcon column="profitLoss" />
                </TableHead>
                <TableHead 
                  className="text-slate-300 cursor-pointer hover:text-white"
                  onClick={() => handleSort('maxDrawdown')}
                >
                  Max Drawdown <SortIcon column="maxDrawdown" />
                </TableHead>
                <TableHead 
                  className="text-slate-300 cursor-pointer hover:text-white"
                  onClick={() => handleSort('maxProfit')}
                >
                  Max Profit <SortIcon column="maxProfit" />
                </TableHead>
                <TableHead 
                  className="text-slate-300 cursor-pointer hover:text-white"
                  onClick={() => handleSort('standardDeviation')}
                >
                  Std Dev <SortIcon column="standardDeviation" />
                </TableHead>
                <TableHead 
                  className="text-slate-300 cursor-pointer hover:text-white"
                  onClick={() => handleSort('sharpeRatio')}
                >
                  Sharpe <SortIcon column="sharpeRatio" />
                </TableHead>
                <TableHead 
                  className="text-slate-300 cursor-pointer hover:text-white"
                  onClick={() => handleSort('totalReturn')}
                >
                  Total Return <SortIcon column="totalReturn" />
                </TableHead>
                <TableHead 
                  className="text-slate-300 cursor-pointer hover:text-white"
                  onClick={() => handleSort('winRate')}
                >
                  Win Rate <SortIcon column="winRate" />
                </TableHead>
              </TableRow>
            </TableHeader>
            <TableBody>
              {sortedMetrics.map((metric) => (
                <TableRow key={metric.year} className="border-slate-600 hover:bg-slate-700">
                  <TableCell className="text-white font-medium">
                    {formatValue('year', metric.year)}
                  </TableCell>
                  <TableCell className={getValueColor('profitLoss', metric.profitLoss)}>
                    {formatValue('profitLoss', metric.profitLoss)}
                  </TableCell>
                  <TableCell className={getValueColor('maxDrawdown', metric.maxDrawdown)}>
                    {formatValue('maxDrawdown', metric.maxDrawdown)}
                  </TableCell>
                  <TableCell className={getValueColor('maxProfit', metric.maxProfit)}>
                    {formatValue('maxProfit', metric.maxProfit)}
                  </TableCell>
                  <TableCell className="text-slate-300">
                    {formatValue('standardDeviation', metric.standardDeviation)}
                  </TableCell>
                  <TableCell className={getValueColor('sharpeRatio', metric.sharpeRatio)}>
                    {formatValue('sharpeRatio', metric.sharpeRatio)}
                  </TableCell>
                  <TableCell className={getValueColor('totalReturn', metric.totalReturn)}>
                    {formatValue('totalReturn', metric.totalReturn)}
                  </TableCell>
                  <TableCell className={getValueColor('winRate', metric.winRate)}>
                    {formatValue('winRate', metric.winRate)}
                  </TableCell>
                </TableRow>
              ))}
            </TableBody>
          </Table>
        </div>
      </CardContent>
    </Card>
  );
};
