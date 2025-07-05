import { useState, useEffect } from 'react';
import { Button } from '@/components/ui/button';
import { Card, CardContent, CardHeader, CardTitle } from '@/components/ui/card';
import { CommoditySelector } from './CommoditySelector';
import { MonthSelector } from './MonthSelector';
import { DateRangeSelector } from './DateRangeSelector';
import { Commodity, SpreadAnalysisParams, AnalysisData } from '@/types/analysis';
import { apiService, ApiError } from '@/services/apiService';
import { useApi } from '@/hooks/useApi';
import { Loader, AlertCircle, Wifi, WifiOff } from 'lucide-react';
import { toast } from '@/hooks/use-toast';
import { Alert, AlertDescription } from '@/components/ui/alert';

interface SpreadAnalysisFormProps {
  onAnalysisComplete: (data: AnalysisData) => void;
}

export const SpreadAnalysisForm = ({ onAnalysisComplete }: SpreadAnalysisFormProps) => {
  const [commodities, setCommodities] = useState<Commodity[]>([]);
  const [selectedCommodity, setSelectedCommodity] = useState<string>('');
  const [selectedMonth1, setSelectedMonth1] = useState<string>('');
  const [selectedMonth2, setSelectedMonth2] = useState<string>('');
  const [startDate, setStartDate] = useState<Date | undefined>();
  const [endDate, setEndDate] = useState<Date | undefined>();
  const [isOnline, setIsOnline] = useState(navigator.onLine);

  // API hooks
  const {
    data: commoditiesData,
    loading: loadingCommodities,
    error: commoditiesError,
    execute: fetchCommodities
  } = useApi(() => apiService.getCommodities());

  const {
    loading: runningAnalysis,
    error: analysisError,
    execute: executeAnalysis
  } = useApi((params) => apiService.runSpreadAnalysis(params));

  // Network status monitoring
  useEffect(() => {
    const handleOnline = () => setIsOnline(true);
    const handleOffline = () => setIsOnline(false);

    window.addEventListener('online', handleOnline);
    window.addEventListener('offline', handleOffline);

    return () => {
      window.removeEventListener('online', handleOnline);
      window.removeEventListener('offline', handleOffline);
    };
  }, []);

  // Load commodities on mount
  useEffect(() => {
    fetchCommodities().catch((error) => {
      console.error('Failed to load commodities:', error);
      
      let errorMessage = 'Failed to load commodities';
      if (error instanceof ApiError) {
        if (error.status >= 500) {
          errorMessage = 'Server error. Please try again later.';
        } else if (error.status === 0 || error.errorCode === 'TIMEOUT') {
          errorMessage = 'Connection error. Please check your internet connection.';
        }
      }
      
      toast({
        title: "Error",
        description: errorMessage,
        variant: "destructive",
      });
    });
  }, []);

  // Update commodities state when data is loaded
  useEffect(() => {
    if (commoditiesData) {
      setCommodities(commoditiesData);
    }
  }, [commoditiesData]);

  const handleRunAnalysis = async () => {
    if (!selectedCommodity || !selectedMonth1 || !selectedMonth2 || !startDate || !endDate) {
      toast({
        title: "Validation Error",
        description: "Please fill in all required fields.",
        variant: "destructive",
      });
      return;
    }

    if (selectedMonth1 === selectedMonth2) {
      toast({
        title: "Validation Error",
        description: "Please select different months for the spread calculation.",
        variant: "destructive",
      });
      return;
    }

    if (startDate >= endDate) {
      toast({
        title: "Validation Error",
        description: "End date must be after start date.",
        variant: "destructive",
      });
      return;
    }

    if (!isOnline) {
      toast({
        title: "Connection Error",
        description: "You are offline. Please check your internet connection.",
        variant: "destructive",
      });
      return;
    }

    const params: SpreadAnalysisParams = {
      commodity: selectedCommodity,
      month1: selectedMonth1,
      month2: selectedMonth2,
      startDate: startDate.toISOString().split('T')[0],
      endDate: endDate.toISOString().split('T')[0],
    };

    try {
      const analysisData = await executeAnalysis(params);
      onAnalysisComplete(analysisData);
      
      toast({
        title: "Analysis Complete",
        description: "Spread analysis has been successfully completed.",
      });
    } catch (error) {
      console.error('Analysis failed:', error);
      
      let errorMessage = 'Failed to run analysis. Please try again.';
      if (error instanceof ApiError) {
        if (error.status === 400) {
          errorMessage = error.message; // Show validation errors from backend
        } else if (error.status >= 500) {
          errorMessage = 'Server error. Please try again later.';
        } else if (error.errorCode === 'TIMEOUT') {
          errorMessage = 'Analysis timeout. Please try with a smaller date range.';
        }
      }
      
      toast({
        title: "Analysis Failed",
        description: errorMessage,
        variant: "destructive",
      });
    }
  };

  const selectedCommodityData = commodities.find(c => c.symbol === selectedCommodity);
  const canRunAnalysis = selectedCommodity && selectedMonth1 && selectedMonth2 && 
                        startDate && endDate && !runningAnalysis && isOnline;

  // Show connection status
  const connectionStatus = !isOnline ? (
    <Alert className="mb-4 border-red-600 bg-red-50">
      <WifiOff className="h-4 w-4" />
      <AlertDescription>
        You are currently offline. Please check your internet connection.
      </AlertDescription>
    </Alert>
  ) : null;

  // Show error state for commodities loading
  if (commoditiesError && !loadingCommodities) {
    return (
      <Card className="bg-slate-800 border-slate-700">
        <CardContent className="p-8 text-center">
          <AlertCircle className="mx-auto mb-4 text-red-400" size={32} />
          <p className="text-slate-300 mb-4">Failed to load commodities</p>
          <Button onClick={() => fetchCommodities()}>
            Try Again
          </Button>
        </CardContent>
      </Card>
    );
  }

  // Show loading state
  if (loadingCommodities) {
    return (
      <Card className="bg-slate-800 border-slate-700">
        <CardContent className="p-8 text-center">
          <Loader className="animate-spin mx-auto mb-4 text-cyan-400" size={32} />
          <p className="text-slate-300">Loading commodities...</p>
        </CardContent>
      </Card>
    );
  }

  return (
    <>
      {connectionStatus}
      <Card className="bg-slate-800 border-slate-700 mb-8">
        <CardHeader className="flex flex-row items-center justify-between">
          <div>
            <CardTitle className="text-white text-xl">Analysis Parameters</CardTitle>
          </div>
          <div className="flex items-center gap-2 text-sm">
            {isOnline ? (
              <div className="flex items-center gap-1 text-green-400">
                <Wifi size={16} />
                <span>Online</span>
              </div>
            ) : (
              <div className="flex items-center gap-1 text-red-400">
                <WifiOff size={16} />
                <span>Offline</span>
              </div>
            )}
          </div>
        </CardHeader>
        <CardContent className="space-y-6">
          {analysisError && (
            <Alert className="border-red-600 bg-red-50">
              <AlertCircle className="h-4 w-4" />
              <AlertDescription className="text-red-800">
                {analysisError}
              </AlertDescription>
            </Alert>
          )}
          
          <div className="grid grid-cols-1 md:grid-cols-4 gap-4">
            <CommoditySelector
              commodities={commodities}
              value={selectedCommodity}
              onChange={setSelectedCommodity}
            />
            
            <MonthSelector
              label="Month 1"
              availableMonths={selectedCommodityData?.available_months || []}
              value={selectedMonth1}
              onChange={setSelectedMonth1}
              disabled={!selectedCommodity}
            />
            
            <MonthSelector
              label="Month 2"
              availableMonths={selectedCommodityData?.available_months || []}
              value={selectedMonth2}
              onChange={setSelectedMonth2}
              disabled={!selectedCommodity}
            />
            
            <Button
              onClick={handleRunAnalysis}
              disabled={!canRunAnalysis}
              className="bg-purple-600 hover:bg-purple-700 text-white font-medium h-10"
            >
              {runningAnalysis ? (
                <>
                  <Loader className="animate-spin mr-2" size={16} />
                  Running...
                </>
              ) : (
                'Run Analysis'
              )}
            </Button>
          </div>
          
          <DateRangeSelector
            startDate={startDate}
            endDate={endDate}
            onStartDateChange={setStartDate}
            onEndDateChange={setEndDate}
          />
        </CardContent>
      </Card>
    </>
  );
};
