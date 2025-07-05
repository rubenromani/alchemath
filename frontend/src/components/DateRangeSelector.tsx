
import { Button } from '@/components/ui/button';
import { Calendar } from '@/components/ui/calendar';
import { Popover, PopoverContent, PopoverTrigger } from '@/components/ui/popover';
import { CalendarIcon } from 'lucide-react';
import { format } from 'date-fns';
import { cn } from '@/lib/utils';

interface DateRangeSelectorProps {
  startDate: Date | undefined;
  endDate: Date | undefined;
  onStartDateChange: (date: Date | undefined) => void;
  onEndDateChange: (date: Date | undefined) => void;
}

export const DateRangeSelector = ({
  startDate,
  endDate,
  onStartDateChange,
  onEndDateChange,
}: DateRangeSelectorProps) => {
  return (
    <div className="space-y-4">
      <label className="text-sm font-medium text-slate-300">Analysis Period</label>
      <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
        <div>
          <label className="text-xs text-slate-400 mb-2 block">Start Date</label>
          <Popover>
            <PopoverTrigger asChild>
              <Button
                variant="outline"
                className={cn(
                  "w-full justify-start text-left font-normal bg-slate-700 border-slate-600 text-white hover:bg-slate-600",
                  !startDate && "text-slate-400"
                )}
              >
                <CalendarIcon className="mr-2 h-4 w-4" />
                {startDate ? format(startDate, "PPP") : "Select start date"}
              </Button>
            </PopoverTrigger>
            <PopoverContent className="w-auto p-0 bg-slate-700 border-slate-600" align="start">
              <Calendar
                mode="single"
                selected={startDate}
                onSelect={onStartDateChange}
                disabled={(date) => {
                  const today = new Date();
                  const maxDate = new Date(today.getFullYear() - 1, 11, 31);
                  return date > maxDate || (endDate && date >= endDate);
                }}
                initialFocus
                className="p-3 pointer-events-auto bg-slate-700 text-white"
              />
            </PopoverContent>
          </Popover>
        </div>
        
        <div>
          <label className="text-xs text-slate-400 mb-2 block">End Date</label>
          <Popover>
            <PopoverTrigger asChild>
              <Button
                variant="outline"
                className={cn(
                  "w-full justify-start text-left font-normal bg-slate-700 border-slate-600 text-white hover:bg-slate-600",
                  !endDate && "text-slate-400"
                )}
              >
                <CalendarIcon className="mr-2 h-4 w-4" />
                {endDate ? format(endDate, "PPP") : "Select end date"}
              </Button>
            </PopoverTrigger>
            <PopoverContent className="w-auto p-0 bg-slate-700 border-slate-600" align="start">
              <Calendar
                mode="single"
                selected={endDate}
                onSelect={onEndDateChange}
                disabled={(date) => {
                  const today = new Date();
                  const maxDate = new Date(today.getFullYear() - 1, 11, 31);
                  return date > maxDate || (startDate && date <= startDate);
                }}
                initialFocus
                className="p-3 pointer-events-auto bg-slate-700 text-white"
              />
            </PopoverContent>
          </Popover>
        </div>
      </div>
    </div>
  );
};
