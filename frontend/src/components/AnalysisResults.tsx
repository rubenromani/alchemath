
import { useState } from 'react';
import { Card, CardContent, CardHeader, CardTitle } from '@/components/ui/card';
import { Tabs, TabsContent, TabsList, TabsTrigger } from '@/components/ui/tabs';
import { Button } from '@/components/ui/button';
import { AnalysisData } from '@/types/analysis';
import { SpreadChart } from './SpreadChart';
import { MetricsTable } from './MetricsTable';
import { YearlyChartsGrid } from './YearlyChartsGrid';

interface AnalysisResultsProps {
  data: AnalysisData;
  onNewAnalysis: () => void;
}

export const AnalysisResults = ({ data, onNewAnalysis }: AnalysisResultsProps) => {
  const [activeTab, setActiveTab] = useState('graph');

  return (
    <div className="space-y-6">
      <Card className="bg-slate-800 border-slate-700">
        <CardHeader className="flex flex-row items-center justify-between">
          <div>
            <CardTitle className="text-white text-xl">Analysis Results</CardTitle>
            <p className="text-slate-300 mt-1">
              {data.params.commodity} • {data.params.month1} - {data.params.month2} • 
              {data.params.startDate} to {data.params.endDate}
            </p>
          </div>
          <Button 
            onClick={onNewAnalysis}
            variant="outline" 
            className="bg-slate-700 border-slate-600 text-white hover:bg-slate-600"
          >
            New Analysis
          </Button>
        </CardHeader>
      </Card>

      <Tabs value={activeTab} onValueChange={setActiveTab} className="w-full">
        <TabsList className="bg-slate-800 border-slate-700 w-full md:w-auto">
          <TabsTrigger 
            value="graph" 
            className="data-[state=active]:bg-cyan-600 data-[state=active]:text-white text-slate-300 hover:text-white"
          >
            Spread Chart
          </TabsTrigger>
          <TabsTrigger 
            value="metrics" 
            className="data-[state=active]:bg-cyan-600 data-[state=active]:text-white text-slate-300 hover:text-white"
          >
            Metrics Table
          </TabsTrigger>
          <TabsTrigger 
            value="yearly" 
            className="data-[state=active]:bg-cyan-600 data-[state=active]:text-white text-slate-300 hover:text-white"
          >
            Year-by-Year
          </TabsTrigger>
        </TabsList>

        <TabsContent value="graph" className="space-y-4">
          <SpreadChart data={data} />
        </TabsContent>

        <TabsContent value="metrics" className="space-y-4">
          <MetricsTable metrics={data.yearlyMetrics} />
        </TabsContent>

        <TabsContent value="yearly" className="space-y-4">
          <YearlyChartsGrid yearlyData={data.yearlySpreadData} />
        </TabsContent>
      </Tabs>
    </div>
  );
};
