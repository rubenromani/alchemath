
import { Card, CardContent, CardHeader, CardTitle } from '@/components/ui/card';
import { LineChart, Line, XAxis, YAxis, ResponsiveContainer, Tooltip } from 'recharts';
import { SpreadDataPoint } from '@/types/analysis';

interface YearlyChartsGridProps {
  yearlyData: { [year: number]: SpreadDataPoint[] };
}

export const YearlyChartsGrid = ({ yearlyData }: YearlyChartsGridProps) => {
  const years = Object.keys(yearlyData).map(Number).sort((a, b) => b - a);

  const CustomTooltip = ({ active, payload, label }: any) => {
    if (active && payload && payload.length) {
      return (
        <div className="bg-slate-700 border border-slate-500 rounded p-2 text-xs">
          <p className="text-slate-300">{label}</p>
          <p className="text-cyan-400">
            Value: {payload[0]?.value?.toFixed(2)}
          </p>
        </div>
      );
    }
    return null;
  };

  return (
    <Card className="bg-slate-800 border-slate-700">
      <CardHeader>
        <CardTitle className="text-white">Year-by-Year Spread Charts</CardTitle>
        <p className="text-slate-300 text-sm">
          Individual spread performance for each year in the analysis period
        </p>
      </CardHeader>
      <CardContent>
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 xl:grid-cols-4 gap-4">
          {years.map((year) => {
            const data = yearlyData[year];
            const yearStart = data[0]?.value || 0;
            const yearEnd = data[data.length - 1]?.value || 0;
            const yearReturn = yearEnd - yearStart;
            const isPositive = yearReturn >= 0;

            return (
              <Card key={year} className="bg-slate-700 border-slate-600">
                <CardHeader className="pb-2">
                  <CardTitle className="text-white text-lg">{year}</CardTitle>
                  <p className={`text-sm ${isPositive ? 'text-green-400' : 'text-red-400'}`}>
                    Return: {yearReturn > 0 ? '+' : ''}{yearReturn.toFixed(2)}
                  </p>
                </CardHeader>
                <CardContent className="pt-0">
                  <div className="h-32">
                    <ResponsiveContainer width="100%" height="100%">
                      <LineChart data={data}>
                        <XAxis 
                          dataKey="date" 
                          hide
                        />
                        <YAxis 
                          hide
                          domain={['dataMin - 5', 'dataMax + 5']}
                        />
                        <Tooltip content={<CustomTooltip />} />
                        <Line 
                          type="monotone" 
                          dataKey="value" 
                          stroke="#06B6D4" 
                          strokeWidth={1.5}
                          dot={false}
                        />
                      </LineChart>
                    </ResponsiveContainer>
                  </div>
                </CardContent>
              </Card>
            );
          })}
        </div>
      </CardContent>
    </Card>
  );
};
