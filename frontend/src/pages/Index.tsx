
import { useState } from 'react';
import { SpreadAnalysisForm } from '@/components/SpreadAnalysisForm';
import { AnalysisResults } from '@/components/AnalysisResults';
import { AnalysisData } from '@/types/analysis';

const Index = () => {
  const [analysisData, setAnalysisData] = useState<AnalysisData | null>(null);
  const [isLoading, setIsLoading] = useState(false);

  const handleAnalysisComplete = (data: AnalysisData) => {
    setAnalysisData(data);
  };

  const handleNewAnalysis = () => {
    setAnalysisData(null);
  };

  return (
    <div className="min-h-screen bg-slate-900 text-white">
      <div className="container mx-auto px-4 py-8">
        <div className="text-center mb-8">
          <h1 className="text-4xl font-bold mb-4 text-white">
            Commodity Spread Analysis
          </h1>
          <p className="text-slate-300 text-lg max-w-2xl mx-auto">
            Perform statistical backtesting analysis on commodity spreads with historical data visualization and comprehensive metrics.
          </p>
        </div>

        <div className="max-w-6xl mx-auto">
          <SpreadAnalysisForm 
            onAnalysisComplete={handleAnalysisComplete}
            isLoading={isLoading}
            setIsLoading={setIsLoading}
          />
          
          {analysisData && (
            <AnalysisResults 
              data={analysisData} 
              onNewAnalysis={handleNewAnalysis}
            />
          )}
        </div>
      </div>
    </div>
  );
};

export default Index;
