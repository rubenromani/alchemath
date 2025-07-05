// src/services/apiService.ts
import { Commodity, SpreadAnalysisParams, AnalysisData } from '@/types/analysis';

// Configuration
const API_BASE_URL = import.meta.env.VITE_API_BASE_URL || 'http://localhost:8000';
const API_VERSION = '/api/v1';

class ApiError extends Error {
  constructor(
    message: string,
    public status: number,
    public errorCode?: string
  ) {
    super(message);
    this.name = 'ApiError';
  }
}

class ApiService {
  private baseUrl: string;

  constructor() {
    this.baseUrl = `${API_BASE_URL}${API_VERSION}`;
  }

  private async handleResponse<T>(response: Response): Promise<T> {
    if (!response.ok) {
      let errorMessage = `HTTP ${response.status}`;
      let errorCode = `HTTP_${response.status}`;
      
      try {
        const errorData = await response.json();
        errorMessage = errorData.message || errorMessage;
        errorCode = errorData.error_code || errorCode;
      } catch {
        // If we can't parse the error response, use the default message
      }
      
      throw new ApiError(errorMessage, response.status, errorCode);
    }

    try {
      return await response.json();
    } catch (error) {
      throw new ApiError('Invalid JSON response', response.status);
    }
  }

  private async fetchWithTimeout(
    url: string, 
    options: RequestInit = {}, 
    timeoutMs: number = 30000
  ): Promise<Response> {
    const controller = new AbortController();
    const timeoutId = setTimeout(() => controller.abort(), timeoutMs);

    try {
      const response = await fetch(url, {
        ...options,
        signal: controller.signal,
        headers: {
          'Content-Type': 'application/json',
          ...options.headers,
        },
      });
      clearTimeout(timeoutId);
      return response;
    } catch (error) {
      clearTimeout(timeoutId);
      if (error instanceof Error && error.name === 'AbortError') {
        throw new ApiError('Request timeout', 408, 'TIMEOUT');
      }
      throw error;
    }
  }

  async getCommodities(): Promise<Commodity[]> {
    console.log('Fetching commodities from API...');
    
    try {
      const response = await this.fetchWithTimeout(`${this.baseUrl}/commodities`);
      const data = await this.handleResponse<Commodity[]>(response);
      
      console.log(`Successfully fetched ${data.length} commodities`);
      return data;
    } catch (error) {
      console.error('Error fetching commodities:', error);
      throw error;
    }
  }

  async getCommodityBySymbol(symbol: string): Promise<Commodity> {
    console.log(`Fetching commodity with symbol: ${symbol}`);
    
    try {
      const response = await this.fetchWithTimeout(`${this.baseUrl}/commodities/${symbol}`);
      const data = await this.handleResponse<Commodity>(response);
      
      console.log(`Successfully fetched commodity: ${data.name}`);
      return data;
    } catch (error) {
      console.error(`Error fetching commodity ${symbol}:`, error);
      throw error;
    }
  }

  async runSpreadAnalysis(params: SpreadAnalysisParams): Promise<AnalysisData> {
    console.log('Starting spread analysis...', params);
    
    try {
      const response = await this.fetchWithTimeout(
        `${this.baseUrl}/analysis/spread`,
        {
          method: 'POST',
          body: JSON.stringify(params),
        },
        60000 // 60 second timeout for analysis
      );
      
      const data = await this.handleResponse<AnalysisData>(response);
      
      console.log('Spread analysis completed successfully');
      return data;
    } catch (error) {
      console.error('Error running spread analysis:', error);
      throw error;
    }
  }

  async healthCheck(): Promise<{ status: string; service: string; version: string }> {
    try {
      const response = await this.fetchWithTimeout(`${API_BASE_URL}/health`);
      return await this.handleResponse(response);
    } catch (error) {
      console.error('Health check failed:', error);
      throw error;
    }
  }
}

// Export singleton instance
export const apiService = new ApiService();
export { ApiError };