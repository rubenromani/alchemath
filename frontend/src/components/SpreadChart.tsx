
import { Card, CardContent, CardHeader, CardTitle } from '@/components/ui/card';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer } from 'recharts';
import { AnalysisData } from '@/types/analysis';

interface SpreadChartProps {
  data: AnalysisData;
}

export const SpreadChart = ({ data }: SpreadChartProps) => {
  // Combine all data series by date for the chart
  const chartData = data.currentYearData.map(point => {
    const date = point.date;
    const result: any = {
      date,
      current: point.value,
    };

    // Add average data points for the same date if they exist
    const avg3Point = data.averageData.avg3Year.find(p => p.date === date);
    const avg5Point = data.averageData.avg5Year.find(p => p.date === date);
    const avg10Point = data.averageData.avg10Year.find(p => p.date === date);
    const avg15Point = data.averageData.avg15Year.find(p => p.date === date);

    if (avg3Point) result.avg3 = avg3Point.value;
    if (avg5Point) result.avg5 = avg5Point.value;
    if (avg10Point) result.avg10 = avg10Point.value;
    if (avg15Point) result.avg15 = avg15Point.value;

    return result;
  });

  const CustomTooltip = ({ active, payload, label }: any) => {
    if (active && payload && payload.length) {
      return (
        <div className="bg-slate-800 border border-slate-600 rounded-lg p-3 shadow-lg">
          <p className="text-slate-300 text-sm mb-2">{label}</p>
          {payload.map((entry: any, index: number) => (
            <p key={index} style={{ color: entry.color }} className="text-sm">
              {entry.name}: {entry.value?.toFixed(2)}
            </p>
          ))}
        </div>
      );
    }
    return null;
  };

  return (
    <Card className="bg-slate-800 border-slate-700">
      <CardHeader>
        <CardTitle className="text-white">Spread Analysis</CardTitle>
        <p className="text-slate-300 text-sm">
          Current year performance vs historical averages
        </p>
      </CardHeader>
      <CardContent>
        <div className="h-96">
          <ResponsiveContainer width="100%" height="100%">
            <LineChart data={chartData}>
              <CartesianGrid strokeDasharray="3 3" stroke="#374151" />
              <XAxis 
                dataKey="date" 
                stroke="#9CA3AF"
                fontSize={12}
                tickFormatter={(value) => new Date(value).toLocaleDateString()}
              />
              <YAxis 
                stroke="#9CA3AF"
                fontSize={12}
              />
              <Tooltip content={<CustomTooltip />} />
              <Legend 
                wrapperStyle={{ color: '#D1D5DB' }}
              />
              <Line 
                type="monotone" 
                dataKey="current" 
                stroke="#06B6D4" 
                strokeWidth={2}
                name="Current Year"
                dot={false}
              />
              <Line 
                type="monotone" 
                dataKey="avg3" 
                stroke="#10B981" 
                strokeWidth={1.5}
                strokeDasharray="5 5"
                name="3-Year Average"
                dot={false}
              />
              <Line 
                type="monotone" 
                dataKey="avg5" 
                stroke="#F59E0B" 
                strokeWidth={1.5}
                strokeDasharray="5 5"
                name="5-Year Average"
                dot={false}
              />
              <Line 
                type="monotone" 
                dataKey="avg10" 
                stroke="#EF4444" 
                strokeWidth={1.5}
                strokeDasharray="5 5"
                name="10-Year Average"
                dot={false}
              />
              <Line 
                type="monotone" 
                dataKey="avg15" 
                stroke="#8B5CF6" 
                strokeWidth={1.5}
                strokeDasharray="5 5"
                name="15-Year Average"
                dot={false}
              />
            </LineChart>
          </ResponsiveContainer>
        </div>
      </CardContent>
    </Card>
  );
};
