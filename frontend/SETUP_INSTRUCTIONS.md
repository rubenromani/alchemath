# Commodity Spread Analysis - Setup Instructions

## Prerequisites
- Node.js 18+ and npm
- Python 3.11+
- Git

## Project Structure
```
commodity-analysis/
├── frontend/          # React frontend (your current code)
├── backend/           # FastAPI backend (new)
└── README.md
```

## Backend Setup

1. **Create backend directory and navigate to it:**
```bash
mkdir backend
cd backend
```

2. **Create Python virtual environment:**
```bash
python -m venv venv

# Activate virtual environment
# On Windows:
venv\Scripts\activate
# On macOS/Linux:
source venv/bin/activate
```

3. **Install dependencies:**
```bash
pip install -r requirements.txt
```

4. **Create environment file:**
```bash
cp .env.example .env
```

5. **Run the backend:**
```bash
# Option 1: Using uvicorn directly
uvicorn app.main:app --host 0.0.0.0 --port 8000 --reload

# Option 2: Using the run script
python run.py
```

The backend will be available at: http://localhost:8000
API documentation: http://localhost:8000/api/v1/docs

## Frontend Setup

1. **Navigate to frontend directory:**
```bash
cd frontend  # or wherever your React app is
```

2. **Install the updated dependencies:**
```bash
npm install concurrently
```

3. **Create environment file:**
```bash
echo "VITE_API_BASE_URL=http://localhost:8000" > .env.local
```

4. **Update your files with the new code:**
- Replace `src/services/mockApiService.ts` with the new `apiService.ts`
- Update `src/types/analysis.ts` with the new types
- Update the `SpreadAnalysisForm` component
- Add the new `useApi` hook

5. **Run the frontend:**
```bash
npm run dev
```

## Running Both Together

**Option 1: Separate terminals**
- Terminal 1: Run backend (`cd backend && python run.py`)
- Terminal 2: Run frontend (`cd frontend && npm run dev`)

**Option 2: Concurrent script (if you added it to package.json)**
```bash
npm run dev:api
```

## Testing the Integration

1. **Health Check:**
   Visit http://localhost:8000/health to verify backend is running

2. **API Documentation:**
   Visit http://localhost:8000/api/v1/docs to explore the API

3. **Frontend:**
   Visit http://localhost:5173 (or your Vite port) to use the application

4. **Test Flow:**
   - Load commodities (should fetch from backend)
   - Select commodity and months
   - Run analysis (should take ~2 seconds)
   - View results

## Troubleshooting

### Backend Issues:
- **Import errors**: Make sure virtual environment is activated
- **Port conflicts**: Change port in `run.py` if 8000 is taken
- **CORS errors**: Check BACKEND_CORS_ORIGINS in .env

### Frontend Issues:
- **API connection errors**: Verify VITE_API_BASE_URL in .env.local
- **Type errors**: Make sure all new types are properly imported

### Network Issues:
- **Connection refused**: Ensure backend is running on correct port
- **Timeout errors**: Check firewall settings

## Development Workflow

1. **Backend changes**: Code will auto-reload with uvicorn --reload
2. **Frontend changes**: Vite will hot-reload automatically
3. **API changes**: Update both Pydantic models and TypeScript types
4. **Testing**: Both services have health check endpoints

## Next Steps

Once everything is working:
1. Set up PostgreSQL database
2. Implement C++ engine integration
3. Add authentication
4. Deploy to production

## Logs and Debugging

- **Backend logs**: Check terminal running Python server
- **Frontend logs**: Check browser console
- **API requests**: Use browser DevTools Network tab
- **API testing**: Use the auto-generated docs at /api/v1/docs